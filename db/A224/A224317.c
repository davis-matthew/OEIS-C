/*
 * A224317  a(n) = a(n-1) + 3 - a(n-1)!.
 * https://oeis.org/A224317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(n)=floor((4*n-1)/3)+sign(n‑1) mod 4 (equivalently for
 *            n≥2: a(n)=((4*n-1)/3+1) % 4)
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t nn = (seq_t)n;
    /* Compute floor((4*nn - 1)/3) + 1, then mod 4 */
    seq_t t = ((4 * nn - 1) / 3 + 1) % 4;
    return t;
}
