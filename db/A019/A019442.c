/*
 * A019442  Numbers m such that a Hadamard matrix of order m exists.
 * https://oeis.org/A019442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(1)=1, a(2)=2, a(n)=4*(n-2) for n>=3
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1U) return (seq_t)1;
    if (n == 2U) return (seq_t)2;
    return (seq_t)4 * (((seq_t)n) - (seq_t)2);
}
