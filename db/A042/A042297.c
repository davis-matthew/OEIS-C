/*
 * A042297  Denominators of continued fraction convergents to sqrt(674).
 * https://oeis.org/A042297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A042297: Denominators of continued fraction convergents to sqrt(674)
   Recurrence: a(n) = 1350*a(n-4) - a(n-8) for n > 7
   Initial values:
     a(0) = 1, a(1) = 1, a(2) = 25, a(3) = 26,
     a(4) = 1325, a(5) = 1351, a(6) = 33749, a(7) = 35100
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle small indices directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)25;
    if (n == 3) return (seq_t)26;
    if (n == 4) return (seq_t)1325;
    if (n == 5) return (seq_t)1351;
    if (n == 6) return (seq_t)33749;
    if (n == 7) return (seq_t)35100;

    /* compute all terms up to n using the recurrence */
    size_t len = (size_t)n + 1UL;
#ifdef __cplusplus
    seq_t *seq = new seq_t[len];
#else
    seq_t *seq = (seq_t *)malloc(len * sizeof(seq_t));
#endif
    if (!seq) return (seq_t)0;  /* allocation failure */

    seq[0] = (seq_t)1;
    seq[1] = (seq_t)1;
    seq[2] = (seq_t)25;
    seq[3] = (seq_t)26;
    seq[4] = (seq_t)1325;
    seq[5] = (seq_t)1351;
    seq[6] = (seq_t)33749;
    seq[7] = (seq_t)35100;

    seq_t factor = (seq_t)1350;
    for (idx_t i = 8; i <= n; ++i) {
        seq[i] = factor * seq[i - 4] - seq[i - 8];
    }

    seq_t result = seq[n];
#ifdef __cplusplus
    delete[] seq;
#else
    free(seq);
#endif
    return result;
}
