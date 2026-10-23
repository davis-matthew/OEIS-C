/*
 * A118698  Smaller of two consecutive semiprimes with the same multiplicative
 *          digital root.
 * https://oeis.org/A118698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bigomega, digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) = the smaller of two consecutive semiprimes
   having the same multiplicative digital root. The algorithm
   scans the integers, keeps the current and previous semiprime,
   and records the smaller member of each qualifying pair. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/bigomega.h"
#include "../utils/digit_product.h"

static seq_t mdr(seq_t x)
{
    while (x >= (seq_t)10) {
        x = digit_product(x);
    }
    return x;
}

seq_t a(idx_t n)
{
    /* Cache of already found sequence values */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* State while scanning integers */
    static uint64_t cur = 2;          /* current candidate to test */
    static seq_t   prev_semiprime = 0;/* previous semiprime found */
    static seq_t   prev_mdr      = 0;/* its multiplicative digital root */

    /* Grow cache to hold at least n terms */
    while (cache_len < n) {
        /* Find next semiprime */
        uint64_t candidate;
        while (true) {
            candidate = cur++;
            if ((size_t)bigomega(candidate) == 2) {
                break;
            }
        }

        /* Compute multiplicative digital root of the semiprime */
        seq_t m = mdr((seq_t)candidate);

        /* If consecutive semiprimes share the same mdr, record the smaller one */
        if (prev_semiprime && m == prev_mdr) {
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                /* If realloc fails the program aborts – no need for error handling */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = prev_semiprime;
        }

        /* Update state for next iteration */
        prev_semiprime = (seq_t)candidate;
        prev_mdr       = m;
    }

    return cache[n - 1];
}
