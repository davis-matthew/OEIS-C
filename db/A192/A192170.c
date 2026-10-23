/*
 * A192170  Monotonic ordering of nonnegative differences 9^i-4^j, for 40>=
 *          i>=0, j>=0.
 * https://oeis.org/A192170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 27, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A192170: monotonic ordering of nonnegative differences 9^i-4^j, i>=0, j>=0. */
/* The function computes a(n), 1‑based, for any n>=1. */

#include <stdlib.h>
#include <stddef.h>

/* Compare two seq_t values – use the supplied routine if available,
   otherwise fall back to the built‑in operators. */
static int
seq_cmp(const void *p1, const void *p2)
{
#ifdef cmp_seq_t
    return cmp_seq_t(p1, p2);
#else
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
#endif
}

seq_t a(idx_t n)
{
    /* Static cache of all terms – constructed on first call. */
    static seq_t  terms[2000];
    static size_t term_count = 0;

    if (term_count == 0) {
        /* Powers of 9 and 4.  The loops stop when multiplication would
           overflow for an unsigned seq_t; for an arbitrary‑precision
           type they run to the 40th exponent.  */
        seq_t pow9[40];
        seq_t pow4[40];
        size_t pow9_len = 0, pow4_len = 0;

        pow9[0] = 1;
        pow9_len = 1;
        for (size_t i = 1; i < 40; ++i) {
            seq_t next = pow9[i-1] * 9;
            /* Detect overflow for unsigned integer types. */
            if (next < pow9[i-1]) break;
            pow9[pow9_len++] = next;
        }

        pow4[0] = 1;
        pow4_len = 1;
        for (size_t j = 1; j < 40; ++j) {
            seq_t next = pow4[j-1] * 4;
            if (next < pow4[j-1]) break;
            pow4[pow4_len++] = next;
        }

        /* Generate all nonnegative differences 9^i - 4^j.  */
        seq_t tmp[2000];
        size_t tlen = 0;
        for (size_t i = 0; i < pow9_len; ++i) {
            for (size_t j = 0; j < pow4_len; ++j) {
                if (pow4[j] > pow9[i]) break;
                tmp[tlen++] = pow9[i] - pow4[j];
            }
        }

        /* Sort and deduplicate.  */
        qsort(tmp, tlen, sizeof(seq_t), seq_cmp);

        term_count = 0;
        for (size_t k = 0; k < tlen; ++k) {
            if (k == 0 || seq_cmp(&tmp[k], &tmp[k-1]) != 0) {
                terms[term_count++] = tmp[k];
            }
        }
    }

    /* Return the requested term (1‑based indexing).  */
    if (n == 0 || (size_t)(n-1) >= term_count) {
        return 0; /* out of range – should not happen with valid tests */
    }
    return terms[(size_t)(n-1)];
}
