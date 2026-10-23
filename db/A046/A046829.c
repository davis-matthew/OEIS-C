/*
 * A046829  Numbers k such that digits of k^2 include digits of k as
 *          subsequence.
 * https://oeis.org/A046829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 19, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   get_digits, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: returns the nth number k (1‑based) such that the decimal
   representation of k² contains the digits of k as a subsequence. */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/grow_seq.h"
#include "../utils/get_digits.h"

seq_t a(idx_t n)
{
    /* Static state: cache of computed terms and next candidate to test. */
    static seq_t *terms = NULL;
    static size_t terms_size = 0;
    static size_t terms_cap = 0;
    static idx_t next_candidate = 0;   /* start checking from 0 */

    /* Ensure we have at least n elements in the cache. */
    while (terms_size < n)
    {
        seq_t k = (seq_t)next_candidate;
        seq_t sq = k * k;

        /* Temporary buffers for decimal digits. */
        uint8_t buf_k[64];
        uint8_t buf_sq[128];
        size_t len_k, len_sq;

        /* Obtain digits of k and k².  get_digits returns most‑significant
           digit first. */
        get_digits(k,   buf_k,   &len_k);
        get_digits(sq,  buf_sq,  &len_sq);

        /* Check whether the digits of k appear in order in the digits of
           k². */
        size_t i = 0;
        for (size_t j = 0; j < len_sq && i < len_k; ++j)
        {
            if (buf_sq[j] == buf_k[i])
                ++i;
        }
        if (i == len_k)   /* subsequence found */
        {
            grow_seq(&terms, &terms_cap, terms_size + 1);
            terms[terms_size++] = k;
        }

        ++next_candidate;
    }

    return terms[n - 1];   /* offset 1: a(1) is the first stored term. */
}
