/*
 * A191241  Reversion of x - x^2 - 2*x^5.
 * https://oeis.org/A191241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..floor((n-1)/3)}
 *            2^j*binomial(n-3*j-1,j)*binomial(2*n-3*j-2,n-1)/n
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* sequence defined for n >= 1 */
    seq_t sum = 0;
    seq_t pw2 = 1;                     /* 2^j */
    idx_t maxj = (n - 1) / 3;          /* floor((n-1)/3) */

    for (idx_t j = 0; j <= maxj; ++j) {
        idx_t a1_n = n - 3 * j - 1;
        idx_t a2_n = 2 * n - 3 * j - 2;

        seq_t b1 = binomial(a1_n, j);              /* binomial(n-3*j-1,j) */
        seq_t b2 = binomial(a2_n, n - 1);          /* binomial(2*n-3*j-2,n-1) */

        seq_t term_num = pw2 * b1 * b2;
        seq_t term = term_num / ((seq_t)n);        /* division is exact */

        sum += term;

        pw2 = mul_small(pw2, 2);                   /* increment 2^j for next j */
    }
    return sum;
}
