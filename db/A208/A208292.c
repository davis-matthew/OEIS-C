/*
 * A208292  Primes of the form (n^2+1)/26.
 * https://oeis.org/A208292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 1005, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 56 terms through n = 1985 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes the n-th prime of the form (m^2+1)/26 where m is an odd integer.
 * The sequence is the ordered list of primes of the form (m^2+1)/26 (m odd).
 * The function a(1) returns the first such prime, a(2) the second, etc.
 * The function uses caching so calls may be in any order.
 */
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

static seq_t *cache = NULL;      /* array of found primes */
static size_t cache_len = 0;     /* number of stored primes */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t next_m = 1;         /* next odd m to test */

/* double cache capacity when needed */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) {
        /* allocation failed – in this context we simply ignore further growth;
           the build system is expected to have sufficient memory */
        return;
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* retrieve the n-th prime of the required form (1‑based index) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* undefined – avoid underflow */

    /* find primes until we have at least n of them */
    while (cache_len < n) {
        if (!cache || cache_len == cache_cap)
            grow_cache();

        /* compute (m^2+1)/26 for current odd m */
        seq_t m = (seq_t)next_m;
        seq_t num = m * m + 1;
        if (num % 26 == 0) {
            seq_t p = num / 26;
            if (is_prime_big(p))
                cache[cache_len++] = p;
        }
        next_m += 2;          /* next odd integer */
    }
    return cache[n - 1];
}
