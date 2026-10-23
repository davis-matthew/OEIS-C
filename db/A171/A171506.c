/*
 * A171506  Coefficients of expansion polynomials related to fish weight
 *          allometric equation: p(x,t)=-Exp[t*x]*(1 - Exp[t/3])^3.
 * https://oeis.org/A171506
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, uned, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 19, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula for OEIS A171506:
   a(n) is the flattened coefficient in the expansion of
   p(x,t) = -exp(t*x)*(1 - exp(t/3))^3.
   With row index r = n-2 and 0‑based position k in that row
   (k = offset‑1), the term is
        a(n) = C(r,k) * 3^k * ( 3^(r-k) - 3*2^(r-k) + 3 )
*/

/* -------------------------------------------------------------- */

#include <stdint.h>
/* seq_t and idx_t are provided by the build environment */

static seq_t binom_uint(idx_t n, idx_t k) {
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply by (n - k + i) then divide by i   */
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* compute base^exp; exp is small (<=~150) */
static seq_t pow_small(seq_t base, idx_t exp) {
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i) {
        res *= base;
    }
    return res;
}

seq_t a(idx_t n) {
    /* a(n) is defined only for n >= 3 (offset 3). Below offset is undefined. */
    if (n < 3)
        return (seq_t)0;

    /* position in the flattened list starting at 1 for a(3) */
    idx_t pos = n - 2;

    /* find minimal r such that (r-2)(r-1)/2 >= pos   */
    idx_t r = 3;  /* smallest possible row index */
    while ( ((r - 2) * (r - 1)) / 2 < pos )
        ++r;

    /* elements preceding row r */
    idx_t prev_total = ((r - 3) * (r - 2)) / 2;
    idx_t offset = pos - prev_total;   /* 1‑based index in the row */
    idx_t k = offset - 1;              /* 0‑based position in the row */

    /* core terms of the formula */
    seq_t bin   = binom_uint(r, k);
    seq_t pow3k = pow_small((seq_t)3, k);
    seq_t pow3rk = pow_small((seq_t)3, r - k);
    seq_t pow2rk = pow_small((seq_t)2, r - k);

    seq_t inner = pow3rk - (pow2rk * (seq_t)3) + (seq_t)3;

    /* multiply all parts together */
    seq_t result = bin * pow3k * inner;

    return result;
}
