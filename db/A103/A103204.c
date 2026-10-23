/*
 * A103204  a(1) = 2, a(2) = 4; a(n) = 2*a(n-1) - 1.
 * https://oeis.org/A103204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 2; a(n) = 3*2^(n-2)+1 (n >= 2)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)2;
    seq_t pow2 = ((seq_t)1) << (n - 2);
    return (seq_t)3 * pow2 + (seq_t)1;
}
