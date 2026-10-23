/*
 * A102147  Second Eulerian transform of 1, 2, 3, 4, 5, ... (A000027).
 * https://oeis.org/A102147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Prod_{i=3..n} (2*i-1) for n>=3, a(1) = 1, a(2) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return 1;
    seq_t result = 1;
    for (idx_t i = 3; i <= n; ++i)
        result *= (seq_t)(2 * i - 1);
    return result;
}
