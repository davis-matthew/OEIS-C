/*
 * A210360  Prime numbers p such that x^2 + x + p produces primes for x = 0..1
 *          but not x = 2.
 * https://oeis.org/A210360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 677, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 999 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: list of primes p such that p is prime,
   p+2 is also prime, and p+6 is composite (not prime). */

#include <stddef.h>
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

seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *cache = NULL;     /* array of found terms */
    static size_t cache_len = 0;    /* number of cached terms */
    static size_t cache_cap = 0;    /* allocated capacity */
    static seq_t next_candidate = 2; /* next integer to test for being a prime */

    /* return cached value if already computed */
    if (n > 0 && (size_t)(n - 1) < cache_len)
        return cache[n - 1];

    /* grow cache until we have at least n terms */
    while (cache_len < n) {
        seq_t p = next_candidate;

        /* find the next qualifying prime */
        for (;;) {
            if (is_prime_big(p)) {
                seq_t p2 = p + (seq_t)2;
                if (is_prime_big(p2)) {
                    seq_t p6 = p + (seq_t)6;
                    if (!is_prime_big(p6)) {
                        /* satisfy all conditions: store p */
                        if (cache_len == cache_cap) {
                            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                            /* assume realloc succeeds */
                            cache = tmp;
                            cache_cap = new_cap;
                        }
                        cache[cache_len++] = p;
                        next_candidate = p + (seq_t)1; /* start from next integer next time */
                        break; /* exit inner loop: found one term */
                    }
                }
            }
            /* move to next integer */
            p = p + (seq_t)1;
            next_candidate = p; /* keep next_candidate updated for next outer run */
        }
    }

    return cache[n - 1];
}
