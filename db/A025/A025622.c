/*
 * A025622  Numbers of form 5^i*6^j, with i, j >= 0.
 * https://oeis.org/A025622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 98, u64 n <= 367,
 *            u128 n <= 1417, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A025622: Numbers of the form 5^i * 6^j (i,j >= 0) in increasing order. */
/* Computation: generate the sequence by a two‑pointer merge starting with 1. */

#include <stdlib.h>
#include <stdint.h>   /* for uint64_t, etc. */

seq_t a(idx_t n)
{
    if (n == 0)  /* Undefined for n < 1, but keep a consistent return value. */
        return 0;

    /* Allocate an array of n elements.  Use `new` in C++ so objects are
       properly constructed; fall back to malloc for plain C.  The
       corresponding deallocation uses delete[] / free. */
#ifdef __cplusplus
    seq_t *seq = new seq_t[n];
#else
    seq_t *seq = (seq_t *)malloc((size_t)n * sizeof(seq_t));
#endif
    if (!seq)  /* Allocation failure – return 0 as a sentinel. */
        return 0;

    seq[0] = 1;                /* a(1) = 1 */
    idx_t i5 = 0;              /* index for next multiple of 5 */
    idx_t i6 = 0;              /* index for next multiple of 6 */
    idx_t cur = 1;             /* current position to fill (0‑based) */

    /* Generate subsequent terms until we have n values stored. */
    while (cur < n) {
        seq_t cand5 = seq[i5] * (seq_t)5;
        seq_t cand6 = seq[i6] * (seq_t)6;

        if (cand5 < cand6) {
            seq[cur++] = cand5;
            ++i5;
        } else if (cand6 < cand5) {
            seq[cur++] = cand6;
            ++i6;
        } else { /* cand5 == cand6 – avoid duplicate */
            seq[cur++] = cand5;
            ++i5;
            ++i6;
        }
    }

    seq_t result = seq[n - 1];   /* n is 1‑based in OEIS terms */

    /* Clean up the allocated memory. */
#ifdef __cplusplus
    delete[] seq;
#else
    free(seq);
#endif

    return result;
}
