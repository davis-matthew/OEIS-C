/*
 * A095796  a(n) = 1 + (26*n+17+7*n^2)*n/2.
 * https://oeis.org/A095796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + (26*n + 17 + 7*n^2)*n/2
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 915, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    // Compute (26*n + 17 + 7*n^2)
    seq_t tmp = ((seq_t)7 * sn * sn)  // 7*n^2
              + ((seq_t)26 * sn)    // 26*n
              + (seq_t)17;          // +17
    // Multiply by n and divide by 2
    seq_t val = sn * tmp;
    val = val / (seq_t)2;
    // Add the constant 1
    val += (seq_t)1;
    return val;
}
