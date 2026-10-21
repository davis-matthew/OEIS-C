/*
 * A096492  Number of distinct terms in continued fraction period of square
 *          root of n.
 * https://oeis.org/A096492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a perfect square, otherwise the number of
 *            distinct terms in the period of the continued fraction expansion
 *            of √n. (OEIS A096492)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   _isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_isqrt_u64.h"

seq_t a(idx_t n) {
    /* Handle square numbers */
    idx_t a0 = _isqrt_u64(n);
    if (a0 * a0 == n) {
        return 1;
    }

    /* Variables for the continued‑fraction algorithm */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a_val = a0;          /* current a_i (before update) */

    /* Dynamic array to collect distinct period terms */
    seq_t *seen = NULL;
    size_t seen_len = 0, seen_cap = 0;

    for (;;) {
        /* Compute next m, d, a using 128‑bit arithmetic to avoid overflow. */
        unsigned __int128 temp = (unsigned __int128)d * a_val - m;
        m = (idx_t)temp;

        temp = (unsigned __int128)n - (unsigned __int128)m * m;
        d = (idx_t)(temp / d);

        a_val = (a0 + m) / d;          /* next a_i */

        /* Record distinct term */
        bool exists = false;
        for (size_t i = 0; i < seen_len; ++i) {
            if (seen[i] == a_val) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            if (seen_len == seen_cap) {
                size_t new_cap = seen_cap ? seen_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(seen, new_cap * sizeof(seq_t));
                if (!tmp) {            /* allocation failure – return 0 */
                    free(seen);
                    return 0;
                }
                seen = tmp;
                seen_cap = new_cap;
            }
            seen[seen_len++] = a_val;
        }

        /* Termination: period ends when a_i = 2*a0 */
        if (a_val == 2 * a0) {
            break;
        }
    }

    seq_t result = (seq_t)seen_len;
    free(seen);
    return result;
}
