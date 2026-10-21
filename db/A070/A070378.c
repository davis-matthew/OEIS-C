/*
 * A070378  a(n) = 5^n mod 28.
 * https://oeis.org/A070378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n mod 28, periodic with period 6 (1,5,25,13,9,17)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* period of 5^n modulo 28 */
    static const seq_t period[6] = { (seq_t)1, (seq_t)5, (seq_t)25,
                                     (seq_t)13, (seq_t)9, (seq_t)17 };
    return period[n % 6];
}
