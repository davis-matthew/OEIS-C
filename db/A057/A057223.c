/*
 * A057223  Number of 4 X n binary matrices without singleton columns up to
 *          row and column permutations.
 * https://oeis.org/A057223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/24 * * [ * C(n+11,11) * + 8 * Σ_{b ≥ 0} C(n-3b+2,2) *
 *            C(b+2,2) * + 6 * Σ_{b ≥ 0} C(n-2b+5,5) * C(b+2,2) * + 3 * Σ_{b ≥
 *            0} C(n-2b+3,3) * C(b+3,3) * + 6 * Σ_{k ≥ 0} (k+1) * S(n-4k) * ]
 *            * where S(m) = ((m/2)+1)^2 if m even * = ((m/2)+1)*((m/2)+2) if
 *            m odd
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 23, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t binom_small(idx_t n, idx_t k)
{
    /* Computes C(n,k) exactly for small k (≤ 11) using integer arithmetic. */
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;          /* use symmetry to reduce steps */
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        /* res = res * (n - k + i) / i  ; division is exact */
        res = (res * (seq_t)(n - k + i)) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t total = (seq_t)0;

    /* Term T1 */
    total += binom_small(n + (idx_t)11, (idx_t)11);

    /* Term T2 */
    seq_t sum2 = (seq_t)0;
    for (idx_t b = (idx_t)0; b <= n / (idx_t)3; ++b) {
        seq_t left  = binom_small(n - (idx_t)(3 * b) + (idx_t)2, (idx_t)2);
        seq_t right = binom_small(b + (idx_t)2, (idx_t)2);
        sum2 += left * right;
    }
    total += (seq_t)8 * sum2;

    /* Term T3 */
    seq_t sum3 = (seq_t)0;
    for (idx_t b = (idx_t)0; b <= n / (idx_t)2; ++b) {
        seq_t left  = binom_small(n - (idx_t)(2 * b) + (idx_t)5, (idx_t)5);
        seq_t right = binom_small(b + (idx_t)2, (idx_t)2);
        sum3 += left * right;
    }
    total += (seq_t)6 * sum3;

    /* Term T4 */
    seq_t sum4 = (seq_t)0;
    for (idx_t b = (idx_t)0; b <= n / (idx_t)2; ++b) {
        seq_t left  = binom_small(n - (idx_t)(2 * b) + (idx_t)3, (idx_t)3);
        seq_t right = binom_small(b + (idx_t)3, (idx_t)3);
        sum4 += left * right;
    }
    total += (seq_t)3 * sum4;

    /* Term T5 */
    seq_t sum5 = (seq_t)0;
    for (idx_t k = (idx_t)0; k <= n / (idx_t)4; ++k) {
        idx_t m = n - (idx_t)(4 * k);           /* remaining exponent after 4k */
        idx_t t = m / (idx_t)2;                /* floor(m/2)                    */
        seq_t S;
        if (m % (idx_t)2 == (idx_t)0) {         /* even m */
            S = (seq_t)(t + (idx_t)1) * (seq_t)(t + (idx_t)1);
        } else {                               /* odd m */
            S = (seq_t)(t + (idx_t)1) * (seq_t)(t + (idx_t)2);
        }
        sum5 += (seq_t)(k + (idx_t)1) * S;
    }
    total += (seq_t)6 * sum5;

    /* Final division by 24 */
    total /= (seq_t)24;

    return total;
}
