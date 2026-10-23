/*
 * A100710  Characterized by a(n) XOR (a(n) + 1) = a(n) - n.
 * https://oeis.org/A100710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 2; a(2n) = 2*a(n) + 1; a(2n+1) = 2n + 2 (i.e. a(n)=n+1
 *            when n is odd)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used in the offset‑1 range, but safe */
    if (n & 1)                      /* odd n: a(n) = n + 1 */
        return (seq_t)n + 1;
    /* even n: a(n) = 2*a(n/2) + 1 */
    seq_t half = a(n >> 1);
    return (half << 1) + 1;
}
