/*
 * A047304  Numbers not divisible by 7.
 * https://oeis.org/A047304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - 1 + floor((n+5)/6)
 * Range:     u8 n <= 200, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* index is 1‑based; return 0 for n<1 as safety */
    seq_t N  = (seq_t)n;
    seq_t res = N - (seq_t)1 + (N + (seq_t)5) / (seq_t)6;
    return res;
}
