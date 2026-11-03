/*
 * A227508  Numbers n such that n! is divisible by R(n), the digit reversal of
 *          n.
 * https://oeis.org/A227508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A227508: numbers n whose decimal reversal R(n) divides n!.
   This implementation searches successive integers, keeps a cache of
   already found terms, and returns the n‑th term (offset 1). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Return true iff the decimal reversal of m divides m! */
static bool rev_divides_factorial(idx_t m)
{
    /* Compute decimal reverse of m */
    idx_t rev = 0;
    idx_t t = m;
    while (t > 0) {
        rev = rev * 10 + t % 10;
        t /= 10;
    }
    if (rev == 0) return true;          // m > 0, so rev > 0

    /* Factor rev and test each prime power against exponent in m! */
    idx_t tmp = rev;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            unsigned int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            /* Legendre’s formula for exponent of p in m! */
            idx_t exp = 0;
            idx_t d = p;
            while (d <= m) {
                exp += m / d;
                if (d > m / p) break;      /* prevent overflow of d *= p */
                d *= p;
            }
            if (exp < e) return false;
        }
    }
    if (tmp > 1) {  /* remaining prime factor */
        idx_t p = tmp;
        idx_t exp = 0;
        idx_t d = p;
        while (d <= m) {
            exp += m / d;
            if (d > m / p) break;
            d *= p;
        }
        if (exp == 0) return false;
    }
    return true;
}

/* Return the n-th term (offset 1) of A227508. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* undefined offset */

    /* Static cache of previously found terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0, found = 0;
    static idx_t current = 1;

    while (found < n) {
        if (rev_divides_factorial(current)) {
            if (found >= cache_size) {
                size_t new_cap = cache_size ? cache_size << 1 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();
                cache = tmp;
                cache_size = (idx_t)new_cap;
            }
            cache[found++] = (seq_t)current;
        }
        ++current;
    }
    return cache[n - 1];
}
