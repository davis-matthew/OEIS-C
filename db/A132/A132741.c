/*
 * A132741  Largest divisor of n having the form 2^i*5^j.
 * https://oeis.org/A132741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 2^v2(n) * 5^v5(n)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    idx_t m = n;

    /* factor out all powers of 2 */
    while ((m & 1) == 0) {
        res *= (seq_t)2;
        m >>= 1;
    }

    /* factor out all powers of 5 */
    while (m % 5 == 0) {
        res *= (seq_t)5;
        m /= 5;
    }

    return res;
}
