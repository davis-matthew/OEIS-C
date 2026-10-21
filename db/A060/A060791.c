/*
 * A060791  a(n) = n / gcd(n,5).
 * https://oeis.org/A060791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n / gcd(n,5)
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to sequence type once */
    seq_t val = (seq_t)n;

    /* gcd(n,5) is 5 if n is divisible by 5, otherwise 1 */
    if (n % (idx_t)5 == 0) {
        val /= (seq_t)5;
    }

    return val;
}
