/*
 * A170475  Number of reduced words of length n in Coxeter group on 34
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^45 = I.
 * https://oeis.org/A170475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A170475: a(0) = 1, a(n) = 34 * 33^(n-1) for n ≥ 1 (geometric progression starting after the first term). */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    /* compute 33^(n-1) by exponentiation by squaring */
    seq_t pow = 1;
    seq_t base = 33;
    idx_t exp = n - 1;
    while (exp) {
        if (exp & 1) pow *= base;
        base *= base;
        exp >>= 1;
    }
    return (seq_t)34 * pow;
}
