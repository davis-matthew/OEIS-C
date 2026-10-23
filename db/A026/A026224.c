/*
 * A026224  Numbers n such that t(n) = s(n) + 1, where s = A026136, t =
 *          A026142.
 * https://oeis.org/A026224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A026224: numbers n such that t(n) = s(n) + 1
 * The first term is 2; every other term is of the form
 *   3^i * (3k + 1) + 1   (i >= 1, k >= 0).
 * The function returns the n‑th term (1‑based) by generating and
 * merging these increasing sequences. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for 0‑based index */
    if (n == 1) return (seq_t)2;     /* first term */

    /* Upper bound that contains at least n terms.
       Roughly one term per six integers; add a safety margin. */
    idx_t bound = n * 6 + 10;

    /* Compute powers of 3 up to the largest needed. */
    idx_t pow3[64];                   /* enough for all small bounds */
    size_t m = 0;
    pow3[m++] = 1;                    /* 3^0 */
    while (pow3[m - 1] <= bound / 3 && m < sizeof(pow3) / sizeof(pow3[0])) {
        pow3[m++] = pow3[m - 1] * 3;  /* next power of 3 */
    }

    /* Number of sequence indices i >= 1 we will use. */
    size_t count = (m > 1) ? m - 1 : 1;

    /* Arrays for the current value of each sequence and its common difference. */
    seq_t *curr = (seq_t *)malloc(count * sizeof(seq_t));
    seq_t *diff = (seq_t *)malloc(count * sizeof(seq_t));
    if (!curr || !diff) {
        free(curr); free(diff);
        return 0;                      /* allocation failure */
    }

    for (size_t idx = 0; idx < count; ++idx) {
        idx_t i_seq = idx + 1;                             /* i >= 1 */
        curr[idx]  = (seq_t)(pow3[i_seq] + 1);             /* first term 3^i + 1 */
        /* common difference for the sequence (next term - current term) */
        diff[idx]  = (seq_t)(pow3[i_seq] * 3);             /* 3^(i+1) */
    }

    seq_t result = 0;
    for (idx_t step = 1; step <= n - 1; ++step) {
        /* Find the smallest current value among all sequences. */
        size_t min_idx = 0;
        seq_t min_val = curr[0];
        for (size_t j = 1; j < count; ++j) {
            if (curr[j] < min_val) {
                min_val = curr[j];
                min_idx = j;
            }
        }
        result = min_val;
        curr[min_idx] += diff[min_idx];
    }

    free(curr);
    free(diff);
    return result;
}
