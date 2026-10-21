/*
 * A203016  Numbers congruent to {1, 2, 3, 4} mod 6, multiplied by 3.
 * https://oeis.org/A203016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(6*floor((n-1)/4) + ((n-1) mod 4) + 1)
 * Range:     u8 n <= 57, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Equivalent to: a(n) = 18*floor((n-1)/4) + 3*((n-1) mod 4) + 3 */

seq_t a(idx_t n)
{
    /* Index n is 1‑based.  We use unsigned arithmetic, so n>=1. */
    idx_t xm1 = n - 1;          /* n-1, safe because n >= 1 */
    idx_t t   = xm1 / 4;        /* floor((n-1)/4)   */
    idx_t r   = xm1 % 4;        /* (n-1) mod 4      */

    /* Compute 18*t + 3*r + 3. */
    seq_t res = (seq_t)18 * t + (seq_t)3 * r + (seq_t)3;
    return res;
}
