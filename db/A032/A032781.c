/*
 * A032781  Floor ( n(n+1)(n+2)...(n+9) / (n+(n+1)+(n+2)+...+(n+9)) ).
 * https://oeis.org/A032781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( prod_{k=0}^{9} (n+k) / (sum_{k=0}^{9} (n+k)) )
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 5, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)n * ((seq_t)10) + 45;
    seq_t prod = 1;
    for (idx_t i = 0; i < 10; ++i) {
        prod *= (seq_t)(n + i);
    }
    return prod / sum;
}
