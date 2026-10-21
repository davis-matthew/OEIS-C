/*
 * A089626  a(n) = 1/h(n) where {h(n)} is the Hankel transform of {t(n)}; t(n)
 *          is defined by the expansion of tan(x)= Sum_n>0, t(n)*x^(2*n-1);
 *          |x|<Pi/2.
 * https://oeis.org/A089626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4*n-3)^1 * (4*n-5)^2 * ... * 3^(2*n-2) * 1^(2*n-1)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast integer power for seq_t */
static inline seq_t pow_seq_t(seq_t b, idx_t e) {
    seq_t r = 1;
    while (e) {
        if (e & 1) r *= b;
        b *= b;
        e >>= 1;
    }
    return r;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Number of terms in the product */
    idx_t m = 2*n - 1;
    seq_t result = 1;
    for (idx_t k = 1; k <= m; ++k) {
        /* current odd factor: 4*n-3 - 2*(k-1) */
        idx_t odd = (4*n - 3) - 2*(k-1);
        seq_t term = pow_seq_t((seq_t)odd, k);
        result *= term;
    }
    return result;
}
