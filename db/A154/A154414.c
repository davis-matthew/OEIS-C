/*
 * A154414  Primes of the form 20*k^2 + 32*k + 13.
 * https://oeis.org/A154414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 55 terms through n = 930 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A154414
 *  a(n) = prime of the form 20*k^2 + 32*k + 13 where k starts from 0
 *  (offset = 1)
 *  The function searches sequentially for primes of the polynomial
 *  and memoises the results for arbitrary access order.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* static buffer for caching the sequence values */
static seq_t *cache   = NULL;   /* array of found primes */
static size_t cache_cap = 0;    /* current capacity of cache */
static size_t cache_len = 0;    /* number of primes stored in cache */
static idx_t  next_k   = 0;     /* next k value to test */

/* helper: grow cache if necessary */
static void ensure_cache_capacity(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
        if (!new_cache) {
            /* In a production code we would handle out‑of‑memory,
               but for the OEIS build we assume success. */
            exit(1);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* helper: generate primes of the form until we have at least n terms */
static void fill_cache_to(idx_t n)
{
    while ((idx_t)cache_len < n) {
        /* compute f(k) = 20*k^2 + 32*k + 13 */
        seq_t k_t = (seq_t)next_k;
        seq_t val = ((seq_t)20 * k_t * k_t) + ((seq_t)32 * k_t) + ((seq_t)13);

        if (is_prime_big(val)) {
            ensure_cache_capacity();
            cache[cache_len++] = val;
        }
        ++next_k;
    }
}

/* --------------  public interface  -------------- */
seq_t a(idx_t n)
{
    if (n < 1) {
        /* behaviour for n < 1 is undefined; return 0 for safety */
        return (seq_t)0;
    }

    fill_cache_to(n);
    return cache[n - 1];
}
