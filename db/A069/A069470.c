/*
 * A069470  a(n) = Sum_{k>=1} floor(n/(k*(k+1)/2)).
 * https://oeis.org/A069470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k>=1} floor(n / (k*(k+1)/2))
 * Range:     u8 n <= 139, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A069470:  a(n) = Σ_{k≥1} ⌊n/(k(k+1)/2)⌋
 * The summation stops when the triangular number T_k = k(k+1)/2 exceeds n,
 * which occurs after roughly √(2n) terms. This O(√n) algorithm is
 * efficient for the required size limits. */
seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 1; ; ++k) {
        seq_t t = (seq_t)k * (k + 1) / 2;
        if (t > (seq_t)n) break;
        sum += (seq_t)n / t;
    }
    return sum;
}
