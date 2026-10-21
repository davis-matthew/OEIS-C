/*
 * A090809  Coefficient of the irreducible character of S_m indexed by
 *          (m-2n+2,2n-2) in the n-th Kronecker power of the representation
 *          indexed by (m-2,2).
 * https://oeis.org/A090809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1) * n * (3*n^2 + n + 10) / 24
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the closed‑form formula from OEIS A090809 */
seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;       /* n as seq_t */
    seq_t n1  = nn - 1;         /* n-1          */
    seq_t t1  = n1 * nn;        /* (n-1)*n      */
    seq_t n2  = nn * nn;        /* n^2          */
    seq_t t3  = 3 * n2;         /* 3*n^2        */
    seq_t t4  = t3 + nn;        /* 3*n^2 + n    */
    seq_t t5  = t4 + 10;        /* 3*n^2 + n + 10 */
    seq_t t   = t1 * t5;        /* (n-1)*n*(3*n^2 + n + 10) */
    return t / 24;              /* exact integer division */
}
