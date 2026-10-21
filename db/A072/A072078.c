/*
 * A072078  Number of 3-smooth divisors of n.
 * https://oeis.org/A072078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = (exponent of 2 in n + 1) * (exponent of 3 in n + 1)
 * Range:     u8 n <= 9842, u16 n <= 9842, u32 n <= 9842, u64 n <= 9842,
 *            u128 n <= 9842, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* Defined only for n >= 1, but guard anyway */
    idx_t e2 = 0, e3 = 0;
    idx_t tmp = n;

    /* Count powers of 2 */
    while ((tmp & 1) == 0) {
        e2++;
        tmp >>= 1;
    }

    /* Count powers of 3 */
    while (tmp % 3 == 0) {
        e3++;
        tmp /= 3;
    }

    return (seq_t)(e2 + 1) * (seq_t)(e3 + 1);
}
