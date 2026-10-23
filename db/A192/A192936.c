/*
 * A192936  Constant term of the reduction by x^2 -> x + 1 of the polynomial
 *          p(n,x) = Product_{k=1..n} (x+k).
 * https://oeis.org/A192936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = constant term of (x+1)(x+2)…(x+n) after replacing x^2 by
 *            x+1. It can be computed by iteratively multiplying in the
 *            quotient ring ℤ[x]/(x²-x-1). Let current value be represented as
 *            a + b·x. Initialization: a = 1, b = 0. For k = 1 … n: a_new =
 *            a·k + b b_new = a + b·(k+1) The required term is...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t a_val = 1;
    seq_t b_val = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t k_seq = (seq_t)k;
        seq_t new_a = a_val * k_seq + b_val;
        seq_t new_b = a_val + b_val * (k_seq + (seq_t)1);
        a_val = new_a;
        b_val = new_b;
    }
    return a_val;
}
