/*
 * A224822  Expansion of phi(-q) * phi(-q^3)^2 in powers of q where phi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A224822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 198, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Expansion of phi(-q) * phi(-q^3)^2 in powers of q (OEIS A224822) */
#include <stdlib.h>
#include <string.h>

static idx_t floor_sqrt(idx_t x) {
    idx_t r = 0;
    while ((r + 1U) * (r + 1U) <= x)
        ++r;
    return r;
}

seq_t a(idx_t n) {
    /* Pre‑compute r3[s] = Σ_{l,m∈Z} (-1)^{l+m} [s = l^2 + m^2] up to s = n/3 */
    idx_t maxS = n / 3;
    if (maxS == 0U) {                 /* special case: only s = 0 */
        seq_t sign = ((n % 3) == 0U) ? n == 0U ? 1 : 0 : 0;
        return (seq_t)(n == 0U ? 1 : (n == 1U ? -2 : 0));
    }

    seq_t *c3 = (seq_t *)malloc((size_t)(maxS + 1) * sizeof(seq_t));
    if (!c3) return 0;                /* allocation failed, return 0 as fallback */
    memset(c3, 0, (size_t)(maxS + 1) * sizeof(seq_t));

    int R = (int)floor_sqrt(maxS);
    for (int l = -R; l <= R; ++l) {
        for (int m = -R; m <= R; ++m) {
            idx_t sum = (idx_t)((l * l) + (m * m));
            if (sum > maxS) continue;
            seq_t sign = (((l + m) & 1) == 0) ? (seq_t)1 : (seq_t)-1;
            c3[sum] += sign;
        }
    }

    seq_t result = 0;
    idx_t K = floor_sqrt(n);
    for (idx_t k = 0; k <= K; ++k) {
        seq_t sign_k = ((k & 1) == 0) ? (seq_t)1 : (seq_t)-1;
        idx_t t_uint = n - k * k;
        seq_t t = (seq_t)t_uint;
        if (t % 3 != 0) continue;
        idx_t s = t_uint / 3;
        seq_t coeff = c3[s];
        seq_t add = sign_k * coeff;
        if (k > 0) add *= 2;
        result += add;
    }
    free(c3);
    return result;
}
