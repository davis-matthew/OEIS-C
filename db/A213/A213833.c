/*
 * A213833  Rectangular array: (row n) = b**c, where b(h) = 3*h-2, c(h) =
 *          2*n-3+2*h, n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A213833: Rectangular array (row n) = b**c with
   b(h)=3*h-2, c(h)=2*n-3+2*h (h≥1), convolution.
   The sequence a(n) is the array read by falling antidiagonals.
   Recurrence: T(n,k)=4*T(n,k-1)-6*T(n,k-2)+4*T(n,k-3)-T(n,k-4),
   with initial values T(n,1)=2*n-1, T(n,2)=10*n-3, T(n,3)=24*n,
   T(n,4)=44*n+14.  */
seq_t a(idx_t n)
{
    /* Find the diagonal d such that d*(d+1)/2 >= n.
       All indices are 1‑based. */
    idx_t d = 1;
    while (d * (d + 1) / 2 < n)
        d++;

    idx_t before = (d - 1) * d / 2;         /* terms before this diagonal */
    idx_t offset = n - before - 1;          /* zero‑based offset within diagonal */
    idx_t i = offset + 1;                   /* row index (1‑based) */
    idx_t j = d - i + 1;                    /* column index (1‑based) */

    /* Compute T(i,j) using the recurrence.  Values fit in seq_t */
    seq_t i_seq = (seq_t)i;

    if (j == 1) return 2 * i_seq - 1;
    if (j == 2) return 10 * i_seq - 3;
    if (j == 3) return 24 * i_seq;
    if (j == 4) return 44 * i_seq + 14;

    seq_t t1 = 2 * i_seq - 1;          /* T(i,1)   */
    seq_t t2 = 10 * i_seq - 3;         /* T(i,2)   */
    seq_t t3 = 24 * i_seq;             /* T(i,3)   */
    seq_t t4 = 44 * i_seq + 14;        /* T(i,4)   */

    for (idx_t k = 5; k <= j; ++k)
    {
        seq_t t = 4 * t4 - 6 * t3 + 4 * t2 - t1;
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = t;
    }
    return t4;
}
