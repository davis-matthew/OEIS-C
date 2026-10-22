/*
 * A007589  Dodecahedral surface numbers: a(0)=0, a(1)=1, a(2)=20, thereafter
 *          2*((3*n-7)^2 + 21).
 * https://oeis.org/A007589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n=0, 1 if n=1, 20 if n=2, 2*((3*n-7)^2 + 21) for n
 *            >= 3
 * Range:     u8 n <= 5, u16 n <= 62, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 20;
    
    seq_t n_val = (seq_t)n;
    seq_t three_n_minus_7 = 3 * n_val - 7;
    return 2 * (three_n_minus_7 * three_n_minus_7 + 21);
}
