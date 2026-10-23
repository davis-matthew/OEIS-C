/*
 * A021030  Decimal expansion of 1/26.
 * https://oeis.org/A021030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = a(n-1) - a(n-3) + a(n-4) for n > 4, with a(0) = 0, a(1) =
 *            3, a(2) = 8, a(3) = 4, a(4) = 6. The sequence is periodic of
 *            period 6 (for n ≥ 1): 3, 8, 4, 6, 1, 5
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    static const seq_t period[6] = {3, 8, 4, 6, 1, 5};
    idx_t idx = (n - 1) % 6;
    return period[idx];
}
