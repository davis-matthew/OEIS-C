/*
 * A173736  a(n) = (10^n + 35)/9 for n>0.
 * https://oeis.org/A173736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^n + 35)/9 Recurrence: a(0) = 4, a(n) = 10*a(n-1) - 35
 *            (n > 0)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    // a(0) = 4, we compute iteratively up to a(n)
    seq_t val = 4;     // initial value for n = 0
    for (idx_t i = 1; i <= n; ++i) {
        val = val * 10; // multiply by 10
        val = val - 35; // subtract 35
    }
    return val;
}
