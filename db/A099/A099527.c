/*
 * A099527  Triangle, read by rows, of trinomial coefficients arranged so that
 *          there are n+1 terms in row n by setting T(n,k) equal to the
 *          coefficient of z^k in (2 + 3*z + z^2)^(n-[k/2]), for n>=k>=0,
 *          where [k/2] is the integer floor of k/2.
 * https://oeis.org/A099527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the n‑th term of the triangle read by rows,
 * where the triangle entry T(N,K) (0 ≤ K ≤ N) is the coefficient
 * of z^K in (2 + 3z + z²)^(N – ⌊K/2⌋).  The sequence a(n) is
 * obtained by enumerating every T(N,K) row by row, starting with
 * T(0,0)=1.
 * 
 * Each term is computed as:
 *   m = N – (K >> 1)
 *   sum over l = max(0, K‑m) .. ⌊K/2⌋ of
 *          C(m, i) * C(m-i, j) * 2^i * 3^j
 *   where j = K – 2*l, i = m – j – l  (i, j, l ≥ 0, i + j + l = m)
 * 
 * The function below implements this directly and works for the
 * arbitrary‑precision build and for any unsigned integer type
 * chosen for seq_t.  All intermediate values are held in seq_t
 * (or larger types only locally in 128‑bit arithmetic for
 * indices), so the computation is exact whenever it fits in
 * seq_t.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* 1. Find the row R such that n lies in row R */
    idx_t R = 0;
    /* t(R) = R*(R+1)/2  <= n < t(R+1) */
    while ((((__int128)(R + 1) * (R + 2)) / 2) <= n) {
        R++;
    }
    /* 2. Column K and exponent m for this row */
    idx_t prev_terms = (idx_t)(((__int128)R * (R + 1)) / 2);
    idx_t K = n - prev_terms;
    idx_t m = R - (K >> 1);

    /* 3. Prepare powers of 2 and 3 lazily */
    static seq_t *pow2 = NULL;
    static size_t pow2_cap = 0;
    static seq_t *pow3 = NULL;
    static size_t pow3_cap = 0;

    if (pow2 == NULL) {
        pow2_cap = 1;
        pow2 = (seq_t *)malloc(pow2_cap * sizeof(seq_t));
        pow2[0] = (seq_t)1;
    }
    if (pow3 == NULL) {
        pow3_cap = 1;
        pow3 = (seq_t *)malloc(pow3_cap * sizeof(seq_t));
        pow3[0] = (seq_t)1;
    }

    while (pow2_cap <= (size_t)m) {
        size_t new_cap = pow2_cap * 2;
        seq_t *tmp = (seq_t *)realloc(pow2, new_cap * sizeof(seq_t));
        if (!tmp) { free(pow2); free(pow3); return 0; }
        pow2 = tmp;
        for (size_t i = pow2_cap; i < new_cap; i++) {
            pow2[i] = pow2[i - 1] * (seq_t)2;
        }
        pow2_cap = new_cap;
    }
    while (pow3_cap <= (size_t)K) {
        size_t new_cap = pow3_cap * 2;
        seq_t *tmp = (seq_t *)realloc(pow3, new_cap * sizeof(seq_t));
        if (!tmp) { free(pow2); free(pow3); return 0; }
        pow3 = tmp;
        for (size_t i = pow3_cap; i < new_cap; i++) {
            pow3[i] = pow3[i - 1] * (seq_t)3;
        }
        pow3_cap = new_cap;
    }

    /* 4. Sum over all admissible l */
    seq_t total = 0;
    /* l ranges from max(0, K‑m) to floor(K/2) */
    int l_min = (K > m) ? (int)(K - m) : 0;
    int l_max = (int)(K >> 1);
    for (int l = l_min; l <= l_max; ++l) {
        idx_t j = K - (idx_t)(2 * l);       /* number of 3z factors */
        idx_t i = m - j - (idx_t)l;        /* number of 2 factors */
        if (i > m || j > m - i) continue;  /* safety check */

        seq_t coeff = binomial(m, i);
        coeff = coeff * binomial(m - i, j);
        coeff = coeff * pow2[(size_t)i];
        coeff = coeff * pow3[(size_t)j];
        total += coeff;
    }
    return total;
}
