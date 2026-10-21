/*
 * A059590  Numbers obtained by reinterpreting base-2 representation of n in
 *          the factorial base: a(n) = Sum_{k>=0} A030308(n,k)*A000142(k+1).
 * https://oeis.org/A059590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{k>=0} bit_k(n) * (k+1)! (bit_k(n) = k-th binary
 *            digit of n)
 * Range:     u8 n <= 31, u16 n <= 255, u32 n <= 4095, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;          /* accumulator for the sum */
    seq_t f   = 1;          /* (k+1)!  starting with 1! = 1 */
    idx_t i   = 0;          /* current bit position */

    for (idx_t bits = n; bits; bits >>= 1, i++)
    {
        if (bits & 1U)          /* if the current binary digit is 1 */
            res += f;           /* add the corresponding factorial */

        f *= (seq_t)(i + 2);    /* next factorial: (k+2)! = (k+1)! * (k+2) */
    }

    return res;
}
