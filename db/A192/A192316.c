/*
 * A192316  G.f.: A(x) = Sum_{n>=0} x^n * (1+x)^A038722(n), where A038722(n) =
 *          floor(sqrt(2*n)+1/2)^2 - n + 1.
 * https://oeis.org/A192316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m>=0} C(A038722(m), n-m) where A038722(m) =
 *            floor(sqrt(2*m)+1/2)^2 - m + 1
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the equivalent g.f. form:
   a(n) = δ_{n,0} + Σ_{k>=1} [ C(q_k, n-p_k) - C(p_k, n-q_k) ]
   with p_k = (k-1)*k/2 + 1, q_k = k*(k+1)/2 + 1.  */

#include <stdint.h>

static inline seq_t binom(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;  /* a(0) = 1 */

    seq_t result = 0;

    /* iterate over the outer index of the g.f. sum */
    for (idx_t k = 1; ; ++k) {
        /* p_k = T_{k-1} + 1 = (k-1)*k/2 + 1 */
        seq_t p = ((seq_t)(k - 1) * (seq_t)k) / 2 + 1;
        /* q_k = T_k + 1 = k*(k+1)/2 + 1 */
        seq_t q = ((seq_t)k * (seq_t)(k + 1)) / 2 + 1;

        if (p > n && q > n) break;   /* no further contributions */

        /* positive term */
        if (n >= p) {
            idx_t r = n - (idx_t)p;
            if (r <= (idx_t)q) {
                result += binom((idx_t)q, r);
            }
        }

        /* negative term */
        if (n >= q) {
            idx_t r2 = n - (idx_t)q;
            if (r2 <= (idx_t)p) {
                result -= binom((idx_t)p, r2);
            }
        }
    }

    return result;
}
