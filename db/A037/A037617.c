/*
 * A037617  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 1,3,2.
 * https://oeis.org/A037617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor((44/333) * 10^n), i.e. the integer formed by the
 *            first n digits of 0.132132…
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t value = 0;
    const seq_t cycle[3] = {1, 3, 2};
    idx_t i;
    for (i = 0; i < n; ++i) {
        value = value * 10 + cycle[i % 3];
    }
    return value;
}
