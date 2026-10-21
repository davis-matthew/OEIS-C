/*
 * A071552  a(n) = (10n)!/n!^10.
 * https://oeis.org/A071552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10n)!/(n!)^10 Recurrence: a(0) = 1 a(n) = a(n-1) *
 *            ∏_{k=10n-9}^{10n} k / n^10 for n ≥ 1
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = 1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t prod = 1;
        seq_t start = (seq_t)10 * i - 9;
        for (int k = 0; k < 10; ++k)
            prod *= start + k;

        seq_t denom = 1;
        seq_t si = (seq_t)i;
        for (int k = 0; k < 10; ++k)
            denom *= si;

        result = result * prod / denom;
    }
    return result;
}
