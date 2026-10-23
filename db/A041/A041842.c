/*
 * A041842  Numerators of continued fraction convergents to sqrt(443).
 * https://oeis.org/A041842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041842
   Recurrence:  a(n) = 884*a(n-2) – a(n-4)
   Initial values:  a(0)=21,  a(1)=442,  a(2)=18585,  a(3)=390727
   (Computed by iterating the recurrence; works for any order of calls) */
#include <stdlib.h>

/* External linkage, as required. */
seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)21;
    if (n == 1) return (seq_t)442;
    if (n == 2) return (seq_t)18585;
    if (n == 3) return (seq_t)390727;

    /* Keep the last four computed values. */
    seq_t v0 = (seq_t)21;          /* a(n-4)  */
    seq_t v1 = (seq_t)442;         /* a(n-3)  */
    seq_t v2 = (seq_t)18585;       /* a(n-2)  */
    seq_t v3 = (seq_t)390727;      /* a(n-1)  */

    /* Compute the recurrence up to the requested index. */
    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = 884 * a(i-2) – a(i-4)  */
        seq_t cur = (seq_t)(((seq_t)884 * v2) - v0);

        /* Shift the window for the next iteration. */
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = cur;
    }

    return v3;   /* a(n) is stored in v3 after the loop */
}
