/*
 * A154356  Primes of the form 25n^2-14n+2 for n >= 0.
 * https://oeis.org/A154356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 49 terms through n = 977 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Prime of polynomial 25(n-1)^2-14(n-1)+2, nth prime in order   */
/*  a(1) = 2, a(2) = 13, a(3) = 557, …                          */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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

static seq_t *prime_cache = NULL;   /* cached prime values          */
static size_t   cache_size  = 0;    /* number of primes stored      */
static size_t   cache_cap   = 0;    /* capacity of prime_cache       */
static idx_t    next_m      = 0;    /* next polynomial index to test*/

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* not in the sequence domain */

    /* Ensure we have at least n primes in the cache. */
    while (cache_size < n) {
        idx_t    m  = next_m;
        seq_t    mm = (seq_t)m;
        seq_t    val = 25 * mm * mm - 14 * mm + 2;

        if (is_prime_big(val)) {
            /* Grow cache if needed. */
            if (cache_size >= cache_cap) {
                cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                prime_cache = mem_resize(seq_t, prime_cache, cache_cap);
            }
            prime_cache[cache_size++] = val;
        }
        ++next_m;
    }
    return prime_cache[n-1];
}
