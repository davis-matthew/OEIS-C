/*
 * A029773  Squares k^2 in which the digits of k appear.
 * https://oeis.org/A029773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 38, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_mask_idx, digit_mask_seq, has_digits_in_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for the sequence A029773: the squares k^2 such that
 * every decimal digit that appears in k also appears in k^2.
 * A(n) = k_n^2 where k_n is the n-th (1‑based) integer satisfying that property.
 * The function memoises previously computed terms to support arbitrary call order.
 */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_mask_idx.h"
#include "../utils/digit_mask_seq.h"
#include "../utils/has_digits_in_square.h"

/* Main entry‑point required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* offset starts at 1; safe guard */

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_k = 0;     /* next k to test for the property */

    /* grow cache until it contains at least n terms */
    while (cache_len < n) {
        /* find next valid k */
        while (!has_digits_in_square(next_k))
            ++next_k;

        /* compute square */
        seq_t sq_val = (seq_t)next_k * (seq_t)next_k;

        /* grow underlying array if needed */
        if (cache_len + 1 > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                abort();                /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = sq_val;
        ++next_k;
    }
    return cache[n - 1];
}
