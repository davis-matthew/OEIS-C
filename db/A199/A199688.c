/*
 * A199688  a(n) = (8*10^n + 1)/3.
 * https://oeis.org/A199688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*10^n + 1)/3 Equivalent recurrence: a(0) = 3 a(n) =
 *            10*a(n-1) – 3 for n ≥ 1 The recurrence uses only small integer
 *            constants and avoids intermediate overflow for all n where the
 *            result fits the underlying seq_t type
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)3;

    seq_t val = (seq_t)3;
    for (idx_t i = 1; i <= n; ++i)
    {
        val = val * (seq_t)10 - (seq_t)3;
    }
    return val;
}
