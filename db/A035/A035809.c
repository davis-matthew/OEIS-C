/*
 * A035809  Coordination sequence for lattice D*_48 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n=0, else a(n) = Σ_{k=1..48} 2^k * binom(48, k) *
 *            binom(n-1, k-1) + 2^48 * binom(n+23, 47)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    const idx_t N = 48;
    idx_t i;

    /* precompute powers of two up to 2^48 */
    seq_t pow2[N + 1];
    pow2[0] = (seq_t)1;
    for (i = 1; i <= N; i++) {
        pow2[i] = pow2[i - 1] * (seq_t)2;
    }
    seq_t pow2_48 = pow2[N];

    /* precompute binomial coefficients binom(48, k) */
    seq_t binom48[N + 1];
    binom48[0] = (seq_t)1;
    for (i = 1; i <= N; i++) {
        binom48[i] = binom48[i - 1] * (seq_t)(N - (i - 1)) / (seq_t)i;
    }

    /* sum1 = Σ_{k=1..min(48,n)} 2^k * binom(48, k) * binom(n-1, k-1) */
    seq_t sum = (seq_t)0;
    idx_t limitK = (n < N) ? n : N;   /* k runs from 1 to limitK */
    seq_t cur_binom_n = (seq_t)1;      /* binom(n-1, k-1) for current k */

    for (i = 1; i <= limitK; i++) {
        seq_t term = pow2[i] * binom48[i];
        term = term * cur_binom_n;
        sum += term;

        /* update cur_binom_n for next k: binom(n-1, k) */
        if (i < limitK) {
            cur_binom_n = cur_binom_n * (seq_t)(n - i) / (seq_t)i;
        }
    }

    /* term2 = 2^48 * binom(n+23, 47) */
    seq_t binom_n_plus = (seq_t)1;
    /* compute binom(n+23, 47) multiplicatively */
    for (i = 1; i <= 47; i++) {
        seq_t numerator = (seq_t)(n + 23 - (47 - i)); /* = n - 24 + i */
        binom_n_plus = binom_n_plus * numerator / (seq_t)i;
    }
    sum += pow2_48 * binom_n_plus;

    return sum;
}
