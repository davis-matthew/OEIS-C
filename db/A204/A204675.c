/*
 * A204675  a(n) = 16*n^2 + 2*n + 1.
 * https://oeis.org/A204675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*n^2 + 2*n + 1 (offset 0)
 * Range:     u8 n <= 3, u16 n <= 63, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    // Convert index to seq_t for calculations
    seq_t nt = (seq_t)n;
    seq_t res = nt * nt;      // n^2
    res *= 16;                // 16 * n^2
    res += nt * 2;            // + 2*n
    res += 1;                 // + 1
    return res;
}
