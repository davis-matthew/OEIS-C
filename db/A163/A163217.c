/*
 * A163217  Number of reduced words of length n in Coxeter group on 34
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A163217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32*(a(n-1)+a(n-2)+a(n-3)) - 528*a(n-4) for n ≥ 5 with
 *            initial terms a(0)=1, a(1)=34, a(2)=1122, a(3)=37026,
 *            a(4)=1221297
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the initial terms directly */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)34;
    if (n == 2u) return (seq_t)1122;
    if (n == 3u) return (seq_t)37026;
    if (n == 4u) return (seq_t)1221297;

    /* Use a sliding window of the last four terms. */
    seq_t a1 = (seq_t)34;           /* a(n-4) for the first computation */
    seq_t a2 = (seq_t)1122;         /* a(n-3) */
    seq_t a3 = (seq_t)37026;        /* a(n-2) */
    seq_t a4 = (seq_t)1221297;      /* a(n-1) */
    seq_t an = 0;                   /* will hold a(n) */

    for (idx_t i = 5u; i <= n; ++i) {
        an = ((seq_t)32) * (a4 + a3 + a2) - ((seq_t)528) * a1;
        /* Shift the window forward */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return an;
}
