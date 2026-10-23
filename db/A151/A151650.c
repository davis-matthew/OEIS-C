/*
 * A151650  Number of permutations of 5 indistinguishable copies of 1..n with
 *          exactly 5 adjacent element pairs in decreasing order.
 * https://oeis.org/A151650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{j=0..5} (-1)^j * binomial(5*n + 1, j) * binomial(10 -
 *            j, 5)^n
 * Range:     u8 none, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k; /* use symmetry to reduce operations */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res *= (seq_t)(n - k + i);
    }
    /* divide by k! (k ≤ 5) */
    static const seq_t fact[6] = {1, 1, 2, 6, 24, 120};
    res /= fact[k];
    return res;
}

static seq_t my_pow_uint(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t total = 5 * n + 1; /* 5*n + 1, safe for n up to 10,000 */

    for (idx_t j = 0; j <= 5; ++j) {
        seq_t combo_part  = binom_small(total, j);          /* C(5*n+1, j) */
        seq_t base_part   = binom_small(10 - j, 5);        /* C(10-j, 5)  */
        seq_t pow_part    = my_pow_uint(base_part, n);     /* (C(10-j,5))^n */
        seq_t term        = combo_part * pow_part;         /* product */

        if (j & 1)  /* odd j -> negative sign */
            sum -= term;
        else
            sum += term;
    }
    return sum;
}
