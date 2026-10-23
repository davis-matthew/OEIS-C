/*
 * A152861  Scaled row sum zero vector recursion:s=7;
 *          v(n)={s^(n+1),s^(n+1)-Sum[s^i,{i,2,n}],s^n,...,-1}/s^2.
 * https://oeis.org/A152861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 15, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A152861 implementation.
   The sequence is defined by concatenating the vectors
   v(k) = {7^(k-1),
           -(7^(k+1) - Σ_{i=2}^k 7^i)/49,
           -7^(k-2), -7^(k-3), … , -1}
   for k = 0,1,2,…  (s = 7).  This function returns a(n)
   for all n ≥ 0. */

#include <math.h>
#include "../utils/big_pow.h"

/* Return the n‑th term of sequence A152861. */
seq_t a(idx_t n)
{
    const seq_t s = (seq_t)7;          /* scaling constant s = 7 */
    const seq_t s_sq = s * s;          /* s^2 = 49 */

    /* Find k such that k*(k+1)/2 ≤ n < (k+1)*(k+2)/2. */
    idx_t k = (idx_t)((sqrt((double)(8 * n + 1)) - 1) / 2);
    while ((k + 1) * (k + 2) / 2 <= n) ++k;
    while (k * (k + 1) / 2 > n) --k;

    idx_t start   = k * (k + 1) / 2;    /* beginning index of v(k) */
    idx_t offset  = n - start;          /* position inside v(k) */

    /* Handle the special case k == 0 (v(0) = {1}). */
    if (k == 0) return (seq_t)1;

    /* offset == 0: first element, 7^(k-1). */
    if (offset == 0)
        return big_pow(s, k - 1);

    /* offset == 1: second element, -(7^(k+1) - Σ_{i=2}^k 7^i)/49. */
    if (offset == 1) {
        seq_t pow_k_plus1 = big_pow(s, k + 1);  /* 7^(k+1) */
        seq_t sum2k = (seq_t)0;                 /* Σ_{i=2}^k 7^i */
        if (k >= 2) {
            seq_t geom_sum = (pow_k_plus1 - (seq_t)1) / (s - (seq_t)1); /* Σ_{i=0}^k 7^i */
            sum2k = geom_sum - (seq_t)1 - s;                           /* remove 7^0 and 7^1 */
        }
        seq_t numerator = pow_k_plus1 - sum2k;
        seq_t val = numerator / s_sq;          /* exact division */
        return -val;
    }

    /* offset ≥ 2: -7^(k-offset). */
    idx_t exp = k - offset;                  /* exponent ≥ 0 */
    seq_t val = big_pow(s, exp);
    return -val;
}
