/*
 * A001285  Thue-Morse sequence: let A_k denote the first 2^k terms; then A_0
 *          = 1 and for k >= 0, A_{k+1} = A_k B_k, where B_k is obtained from
 *          A_k by interchanging 1's and 2's.
 * https://oeis.org/A001285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, core, nice
 * Formula:   a(n) = 1 if popcount(n) is even, 2 if popcount(n) is odd
 *            (Thue-Morse sequence)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    int popcount = __builtin_popcountll((unsigned long long)n);
    return popcount % 2 == 0 ? (seq_t)1 : (seq_t)2;
}
