/*
 * A058985  a(n) = 6 + 33*n + 6*binomial(n, 2) - 28*binomial(n, 3) +
 *          20*binomial(n, 4) - 47*binomial(n, 5).
 * https://oeis.org/A058985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 6 + 33*n + 6*C(n,2) - 28*C(n,3) + 20*C(n,4) - 47*C(n,5)
 * Range:     i8 n <= 5, i16 n <= 12, i32 n <= 91, i64 n <= 200, i128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t binom(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)             /* use symmetry to minimize multiplications */
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* res = res * (n - i + 1) / i;  division is exact at each step */
        res = res * (seq_t)(n - i + 1) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t res = 6;
    res += 33 * (seq_t)n;
    res += 6 * binom(n, 2);
    res -= 28 * binom(n, 3);
    res += 20 * binom(n, 4);
    res -= 47 * binom(n, 5);
    return res;
}
