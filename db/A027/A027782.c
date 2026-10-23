/*
 * A027782  a(n) = 9*(n+1)*binomial(n+2,9)/2.
 * https://oeis.org/A027782
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+1,8) * binomial(n+2,2) (offset 7)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 30, u64 n <= 263,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t binom8(idx_t n) {
    /* Compute C(n,8) for n>=8 */
    seq_t res = 1;
    for (idx_t i = 1; i <= 8; ++i) {
        res = res * (seq_t)(n - 8 + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n < 7) return 0; /* undefined below offset, return 0 */

    /* C(n+1,8) */
    seq_t c1 = binom8(n + 1);

    /* C(n+2,2) = (n+2)(n+1)/2 */
    seq_t c2 = ((seq_t)(n + 2) * (seq_t)(n + 1)) / 2;

    return c1 * c2;
}
