/*
 * A037549  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,2.
 * https://oeis.org/A037549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = digits of base-8 number formed by first n terms of the
 *            repeating sequence 1,2,2. Recurrence: a(1) = 1 a(n) = a(n-1)*8 +
 *            (n%3==1 ? 1 : 2) for n ≥ 2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined for n < 1, return 0 */

    seq_t value = 1;        /* a(1) */
    if (n == 1)
        return value;

    for (idx_t i = 2; i <= n; ++i) {
        value = value * 8 + ((i % 3 == 1) ? 1 : 2);
    }
    return value;
}
