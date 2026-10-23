/*
 * A046830  Digits of n^2 include digits of n as subsequence, n does not end
 *          in 0.
 * https://oeis.org/A046830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   contains_subseq, digit_len, get_digits, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A046830
   a(n) = n-th positive integer m (m does not end in 0) such that
   the decimal representation of m^2 contains the decimal representation
   of m as a subsequence. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/digit_len.h"
#include "../utils/get_digits.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/contains_subseq.h"

seq_t a(idx_t n)
{
    /* static memoisation state */
    static idx_t    cached     = 0;   /* number of terms currently cached   */
    static seq_t   *cache      = NULL;/* dynamic array of cached terms      */
    static size_t  cap        = 0;   /* current capacity of cache          */
    static seq_t   candidate  = 1;   /* next number to consider in search  */

    if (n == 0) return 0; /* undefined for n < 1 */

    /* Grow the cache to hold at least n terms */
    if (cached < n) {
        /* Allocate or expand the cache array */
        if (cache == NULL) {
            cap = n > 1 ? n : 1;
            cache = seq_alloc(cap);
        } else if (n > cap) {
            size_t new_cap = cap;
            while (n > new_cap) new_cap <<= 1;
            cache = seq_realloc(cache, cap, new_cap);
            cap = new_cap;
        }

        /* Temporary digit buffers – large enough for all numbers we handle. */
        static uint8_t cand_buf[2048], sq_buf[2048];
        size_t cand_len, sq_len;

        /* Generate terms until the n-th is found */
        while (cached < n) {
            /* Find next candidate satisfying the property */
            while (true) {
                /* Skip numbers ending in 0 */
                if ((candidate % 10u) != 0u) {
                    seq_t sq = candidate * candidate;

                    /* Get lengths of the candidate and its square */
                    cand_len = digit_len(candidate);
                    sq_len   = digit_len(sq);

                    /* Extract digits into the buffers */
                    get_digits(candidate, cand_buf, &cand_len);
                    get_digits(sq, sq_buf, &sq_len);

                    /* Check subsequence property */
                    if (contains_subseq(sq_buf, sq_len, cand_buf, cand_len))
                        break; /* candidate is accepted */
                }
                ++candidate; /* try next number */
            }

            /* Store the accepted candidate */
            cache[cached++] = candidate;
            ++candidate;    /* start next search from the following number */
        }
    }

    return cache[n - 1];
}
