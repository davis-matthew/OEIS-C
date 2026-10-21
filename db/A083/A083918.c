/*
 * A083918  Number of divisors of n that are congruent to 8 modulo 10.
 * https://oeis.org/A083918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of n that are congruent to 8 modulo 10
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is defined for n >= 1 only */
    seq_t count = 0;
    /* iterate over divisors up to sqrt(n) */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            if ((d % 10) == 8) ++count;
            idx_t e = n / d;
            if (e != d && (e % 10) == 8) ++count;
        }
    }
    return count;
}
