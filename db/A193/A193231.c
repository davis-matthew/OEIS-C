/*
 * A193231  Blue code for n: in binary coding of a polynomial over GF(2),
 *          substitute x+1 for x (see Comments for precise definition).
 * https://oeis.org/A193231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, hear
 * Formula:   a(n) = XOR of (x+1)^e for each set bit e in the binary expansion
 *            of n * (the polynomial (x+1)^e over GF(2) represented as a
 *            binary number)
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res  = 0;
    seq_t mask = 1;          /* (x+1)^e for the current e (starting at e = 0) */
    idx_t t = n;

    while (t) {
        if (t & 1)
            res ^= mask;     /* include this term in the XOR */
        mask = (mask << 1) ^ mask;   /* compute (x+1)^(e+1) */
        t >>= 1;
    }
    return res;
}
