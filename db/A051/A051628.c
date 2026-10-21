/*
 * A051628  Number of digits in decimal expansion of 1/n before the periodic
 *          part begins.
 * https://oeis.org/A051628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, base
 * Formula:   a(n) = max(v2(n), v5(n)) where v2(n) and v5(n) are the highest
 *            powers of 2 and 5 dividing n. This gives the number of digits
 *            before the periodic part in 1/n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Exponents of 2 and 5 in n */
    seq_t e2 = 0, e5 = 0;

    idx_t m = n;

    /* Count powers of 2 */
    while ((m & 1) == 0) {
        e2++;
        m >>= 1;
    }

    /* Count powers of 5 */
    while (m % 5 == 0) {
        e5++;
        m /= 5;
    }

    return e2 > e5 ? e2 : e5;
}
