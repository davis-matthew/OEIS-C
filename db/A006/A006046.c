/*
 * A006046  Total number of odd entries in first n rows of Pascal's triangle:
 *          a(0) = 0, a(1) = 1, a(2k) = 3*a(k), a(2k+1) = 2*a(k) + a(k+1).
 *          a(n) = Sum_{i=0..n-1} 2^wt(i).
 * https://oeis.org/A006046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, look
 * Formula:   a(0) = 0; a(1) = 1; a(n) = 2*a(floor(n/2)) + a(ceiling(n/2)) for
 *            n > 1
 * Range:     u8 n <= 35, u16 n <= 1209, u32 n <= 9888, u64 n <= 9888,
 *            u128 n <= 9888, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    
    idx_t half = n / 2;
    seq_t lower = a(half);
    seq_t upper = a((n + 1) / 2);
    return 2 * lower + upper;
}
