/*
 * A001341  Expansion of e.g.f. 6*exp(x)/(1-x)^4.
 * https://oeis.org/A001341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} n!/(n-j)! * (j+1)*(j+2)*(j+3) (coefficients
 *            of 6*exp(x)/(1-x)^4)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t ratio = 1;  /* n!/(n-j)! */
    idx_t j;
    for (j = 0; j <= n; j++) {
        seq_t c = (seq_t)(j + 1) * (seq_t)(j + 2) * (seq_t)(j + 3);
        result += ratio * c;
        if (j < n)
            ratio *= (seq_t)(n - j);
    }
    return result;
}
