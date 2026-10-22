/*
 * A001710  Order of alternating group A_n, or number of even permutations of
 *          n letters.
 * https://oeis.org/A001710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*a(n-1) for n > 2, with a(0) = a(1) = a(2) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 2) return 1;
    
    seq_t result = 1;
    for (idx_t i = 3; i <= n; i++) {
        result = result * (seq_t)i;
    }
    return result;
}
