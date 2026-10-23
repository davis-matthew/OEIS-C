/*
 * A146135  Positive integers a(n) such that for every integer m==1(mod
 *          4),m>=5, there exists a unique representation of m as a sum of the
 *          form a(l)+4a(s).
 * https://oeis.org/A146135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 64*floor((n-1)/4) + 4*((n-1)%4) + 1
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for n<1, but guard against underflow */
        return 0;
    idx_t m = n - 1;          /* shift to 0‑based index */
    idx_t q = m / 4;          /* quotient of division by 4 */
    idx_t r = m & 3;          /* remainder of division by 4 (mod 4) */
    return (seq_t)64 * (seq_t)q + (seq_t)4 * (seq_t)r + 1;
}
