/*
 * A037610  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 1,2,3.
 * https://oeis.org/A037610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = ⌊(41/333)·10ⁿ⌋ Recurrence: a(1)=1, a(2)=12, a(3)=123,
 *            a(n)=10·a(n‑1)+d[(n‑1) mod 3], where d={1,2,3}
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    const int d[3] = {1, 2, 3};
    for (idx_t i = 0; i < n; ++i) {
        result = result * 10 + (seq_t)d[i % 3];
    }
    return result;
}
