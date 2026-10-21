/*
 * A064108  a(n) = (20^n - 1)/19.
 * https://oeis.org/A064108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (20^n - 1) / 19 Recurrence: a(0) = 0, a(n) = 20 * a(n-1)
 *            + 1 for n ≥ 1 (The division by 19 is omitted because 20 ≡ 1 (mod
 *            19), so 20^n-1 is always divisible by 19.)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 0;
    for (idx_t i = 0; i < n; ++i)
    {
        val = val * 20 + 1;
    }
    return val;
}
