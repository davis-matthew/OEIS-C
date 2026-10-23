/*
 * A041071  Denominators of continued fraction convergents to sqrt(42).
 * https://oeis.org/A041071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041071: Denominators of continued fraction convergents to sqrt(42)
   Recurrence: a(n) = 26*a(n-2) - a(n-4),  with
   a(0)=1, a(1)=2, a(2)=25, a(3)=52. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first four values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)25;
    if (n == 3) return (seq_t)52;

    /* Allocate space for the sequence up to n. */
    seq_t *seq = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!seq) { /* In case of allocation failure, abort. */
        exit(1);
    }

    /* Initialize the base cases. */
    seq[0] = (seq_t)1;
    seq[1] = (seq_t)2;
    seq[2] = (seq_t)25;
    seq[3] = (seq_t)52;

    /* Compute values using the recurrence relation. */
    for (idx_t i = 4; i <= n; ++i) {
        seq[i] = (seq_t)26 * seq[i - 2] - seq[i - 4];
    }

    /* Retrieve and free the computed value. */
    seq_t result = seq[n];
    free(seq);
    return result;
}
