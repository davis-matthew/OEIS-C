/*
 * A001954  a(n) = floor((n+1/2)*(2+sqrt(2))); winning positions in the
 *          2-Wythoff game.
 * https://oeis.org/A001954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+1/2)*(2+sqrt(2)))
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    // Beatty sequence with 2 + sqrt(2)
    // Using convergent sqrt(2) ≈ 19601/13860
    // 2 + sqrt(2) ≈ 47321/13860
    // a(n) = floor((n + 1/2) * 47321/13860)
    //      = floor((2n + 1) * 47321 / 27720)
    
    seq_t n_s = (seq_t)n;
    return ((n_s * 2 + 1) * 47321) / 27720;
}
