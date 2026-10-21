/*
 * A224711  Number of ballot results from n voters that prompt a run-off
 *          election when three candidates vie for two spots on a board.
 * https://oeis.org/A224711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*sum_{k=0}^{floor((n+2)/2)-floor((n+2)/3)-1} C(n,
 *            2*ceil((n-1)/2)-2k) * C(2*ceil((n-1)/2)-2k, ceil((n-1)/2)-k) -
 *            2*C(n,2n/3)*C(2n/3,n/3)*I[3|n]
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the provided binomial(idx_t n, idx_t k) function */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* ceil((n-1)/2) = n/2 for all n >= 0 (unsigned division) */
    const idx_t b_start = n / 2;

    /* upper limit u = floor((n+2)/2) - floor((n+2)/3) - 1 */
    const idx_t tmp1 = (n + 2) / 2;
    const idx_t tmp2 = (n + 2) / 3;
    const long long u_signed = (long long)tmp1 - (long long)tmp2 - 1;

    seq_t sum = 0;

    if (u_signed >= 0) {
        const idx_t u = (idx_t)u_signed;
        for (idx_t k = 0; k <= u; ++k) {
            const idx_t b = b_start - k;          /* b(k)   */
            const idx_t two_b = b << 1;           /* 2*b(k) */
            const seq_t c1 = binomial(n, two_b);   /* C(n,2b) */
            const seq_t c2 = binomial(two_b, b);   /* C(2b,b) */
            sum += c1 * c2;
        }
    }

    seq_t result = sum * 3;          /* 3*sum */

    if (n % 3 == 0) {                /* subtract 2*C(n,2n/3)*C(2n/3,n/3) */
        const idx_t t = n / 3;
        const seq_t c1 = binomial(n, 2 * t);
        const seq_t c2 = binomial(2 * t, t);
        result -= c1 * c2 * 2;
    }

    return result;
}
