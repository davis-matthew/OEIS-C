/*
 * A065359  Alternating bit sum for n: replace 2^k with (-1)^k in binary
 *          expansion of n.
 * https://oeis.org/A065359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, sign
 * Formula:   a(n) = ∑_{k≥0} bit_k(n) · (−1)^k (alternating bit sum – replace
 *            each 2^k in the binary expansion of n with (−1)^k)
 * Range:     i8 n <= 9946, i16 n <= 9946, i32 n <= 9946, i64 n <= 9946,
 *            i128 n <= 9946, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res  = 0;   /* result accumulator */
    seq_t sign = 1;   /* sign for the current bit position, starts with +1 (k = 0) */

    while (n) {
        if (n & 1)       /* add the sign if the current bit is 1 */
            res += sign;
        sign = -sign;    /* next bit position alternates sign */
        n >>= 1;         /* move to the next higher bit */
    }

    return res;
}
