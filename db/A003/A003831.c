/*
 * A003831  Order of universal Chevalley group D_n (4).
 * https://oeis.org/A003831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^(n(n-1)) * ∏_{i=1}^{n-1} (4^i + 1) * ∏_{i=1}^{n} (4^i -
 *            1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    seq_t b = base;
    while (exp > 0) {
        if (exp % 2 == 1) result *= b;
        b *= b;
        exp /= 2;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t result = pow_seq(4, n * (n - 1));
    seq_t pow_4 = 4;
    
    for (idx_t i = 1; i <= n; i++) {
        if (i < n) result *= (pow_4 + 1);
        result *= (pow_4 - 1);
        pow_4 *= 4;
    }
    
    return result;
}
