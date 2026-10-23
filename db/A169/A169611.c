/*
 * A169611  Number of prime divisors of n that are not greater than 3, counted
 *          with multiplicity.
 * https://oeis.org/A169611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of prime divisors of n that are not greater than
 *            3, counted with multiplicity. Equivalently, a(n) = v₂(n) + v₃(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t m = n;

    /* Count powers of 2 */
    while ((m & 1) == 0) {
        m >>= 1;
        count++;
    }

    /* Count powers of 3 */
    while (m % 3 == 0) {
        m /= 3;
        count++;
    }

    return count;
}
