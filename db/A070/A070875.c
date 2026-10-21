/*
 * A070875  Binary expansion is 1x100...0 where x = 0 or 1.
 * https://oeis.org/A070875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5*2^(n/2) for even n, 7*2^(n/2) for odd n
 * Range:     u8 n <= 11, u16 n <= 27, u32 n <= 59, u64 n <= 123,
 *            u128 n <= 251, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n/2 is the power of 2 to multiply 5 or 7 by */
    idx_t k = n >> 1;
    if ((n & 1) == 0) {
        /* even n: a(2k) = 5 * 2^k */
        return (seq_t)5 << k;
    } else {
        /* odd n: a(2k+1) = 7 * 2^k */
        return (seq_t)7 << k;
    }
}
