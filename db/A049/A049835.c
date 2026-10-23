/*
 * A049835  a(n) = Sum_{k=1..n} T(n,k), array T as in A049834.
 * https://oeis.org/A049835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (sum of quotients from Euclidean algorithm
 *            for (n,k))
 * Range:     u8 n <= 24, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t x = k;
        idx_t y = n;
        seq_t s = 0;
        while (y != 0) {
            s += x / y;
            idx_t r = x % y;
            x = y;
            y = r;
        }
        total += s;
    }
    return total;
}
