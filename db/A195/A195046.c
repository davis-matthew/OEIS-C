/*
 * A195046  Concentric 15-gonal numbers.
 * https://oeis.org/A195046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*n^2/4 + 11*((-1)^n - 1)/8
 * Range:     u8 n <= 8, u16 n <= 132, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent integer arithmetic: even n -> 15*(n/2)^2, odd n -> 15*k^2+15*k+1 where k=(n-1)/2 */
seq_t a(idx_t n)
{
    if ((n & 1ULL) == 0) {
        seq_t k = (seq_t)(n >> 1);
        return (seq_t)15 * k * k;
    } else {
        seq_t k = (seq_t)((n - 1) >> 1);
        return (seq_t)15 * k * k + (seq_t)15 * k + (seq_t)1;
    }
}
