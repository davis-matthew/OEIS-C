/*
 * A103719  Column m=2 sequence of triangle A103718(n,m), n >= 0 (without
 *          leading zeros).
 * https://oeis.org/A103719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |S1(n+3,3)| + |S1(n+3,4)| * where S1 is the unsigned
 *            Stirling number of the first kind (A048994)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* We need the values S1(n+3,3) and S1(n+3,4).  Compute them
       with the recurrence c(n,k)=c(n‑1,k‑1)+(n‑1)*c(n‑1,k),
       only for k=0..4.  We keep a small array of 5 entries,
       updating it in reverse order so that each new entry uses
       only values from the previous row. */
    seq_t c[5] = {0};
    c[0] = 1;                       /* c(0,0)=1 */

    idx_t N = n + 3;
    for (idx_t i = 1; i <= N; ++i)
    {
        seq_t mult = (seq_t)(i - 1); /* cast to seq_t for multiplication */
        for (idx_t k = 4; k >= 1; --k)
            c[k] = c[k - 1] + mult * c[k];
        c[0] = 0;                   /* c(i,0)=0 for i>0 */
    }

    return c[3] + c[4];
}
