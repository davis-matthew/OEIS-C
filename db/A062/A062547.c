/*
 * A062547  a(n) is least odd integer not a partial sum of 1, 3, ..., a(n-1).
 * https://oeis.org/A062547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*3^⌊n/2⌋ - (-1)^n Initial values: a(0)=1, a(1)=3
 *            Recurrence is not used; the closed‑form gives all terms directly
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute e = floor(n/2)   (for odd n, e = (n-1)/2; for even n, e = n/2). */
    idx_t e = n >> 1;      /* divide by 2 using right shift */

    /* Compute 3^e in a loop.  Working with seq_t allows arbitrary precision. */
    seq_t pow = (seq_t)1;
    for (idx_t i = 0; i < e; ++i)
        pow = pow * (seq_t)3;

    /* 2 * 3^e */
    seq_t result = pow * (seq_t)2;

    /* Subtract (-1)^n:
       - if n is even, (-1)^n == 1, so subtract 1.
       - if n is odd,  (-1)^n == -1, so subtract(-1) == +1. */
    if ((n & (idx_t)1) == (idx_t)0)   /* even */
        result = result - (seq_t)1;
    else                              /* odd */
        result = result + (seq_t)1;

    return result;
}
