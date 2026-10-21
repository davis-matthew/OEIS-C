/*
 * A212561  Number of (w,x,y,z) with all terms in {1,...,n} and w + x = 2y +
 *          2z.
 * https://oeis.org/A212561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(2*n^2 + 1 - (-1)^n)/8 (OEIS A212561)
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)            /* a(0) = a(1) = 0 */
        return (seq_t)0;

    seq_t nn = (seq_t)n;           /* cast index to seq_t for arithmetic */
    seq_t part = (seq_t)2 * nn * nn; /* 2*n^2, always non‑negative */
    seq_t s   = nn - 1;           /* (n-1), safe because n >= 2 */

    seq_t numerator;
    /* For odd n: term = (n-1)*(2*n^2 + 2)
       For even n: term = (n-1)*(2*n^2)            */
    if (n & 1)  /* n is odd */
        numerator = s * (part + (seq_t)2);
    else
        numerator = s * part;

    return numerator / (seq_t)8;
}
