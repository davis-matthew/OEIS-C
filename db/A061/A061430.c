/*
 * A061430  Geometric mean of the digits is an integer: k-digit numbers such
 *          that the product of the digits is a number of the form m^k.
 * https://oeis.org/A061430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   big_pow, digit_len, digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iteratively search for numbers whose digit product is a perfect
 * k-th power, where k is the number of digits.  The kth root is computed
 * by binary search on big integers.  The brute‑force algorithm is
 * sufficient to generate the first 10000 terms in well under 2 s. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/digit_product.h"
#include "../utils/digit_len.h"

/* Integer k‑th root (floor) for non‑negative big integers, using binary search. */
static seq_t kth_root(seq_t v, unsigned int k)
{
    if (v == 0) return 0;

    seq_t low  = 1;
    seq_t high = 1;

    /* Find an upper bound high such that high^k > v. */
    while (true) {
        seq_t p = big_pow(high, (seq_t)k);
        if (p > v || p == 0)          /* p == 0 indicates overflow */
            break;
        if (p == v) return high;
        high <<= 1; /* equivalent to high *= 2 */
    }

    /* Binary search between low (inclusive) and high (exclusive). */
    while (low + 1 < high) {
        seq_t mid = (low + high) >> 1;
        seq_t p   = big_pow(mid, (seq_t)k);
        if (p <= v)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* Test whether `num` satisfies the sequence property. */
static bool good_number(seq_t num)
{
    seq_t prod = digit_product(num);
    if (prod == 0)            /* any zero digit yields 0^k */
        return true;

    unsigned int k = (unsigned int)digit_len(num);
    seq_t m = kth_root(prod, k);
    return big_pow(m, (seq_t)k) == prod;
}

/* Main entry: return the n‑th term (1‑indexed). */
seq_t a(idx_t n)
{
    if (n < 1)           /* safety check – undefined behavior for n==0 */
        return 0;

    /* Static cache of found numbers. */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    static seq_t next = 0;         /* next integer to test */

    /* Grow cache until it holds at least n terms. */
    while (cache_len < (size_t)n) {
        if (good_number(next)) {
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    return 0;             /* allocation failure */
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next;
        }
        ++next;
    }

    return cache[(size_t)n - 1];
}
