/*
 * A035713  Coordination sequence for 18-dimensional cubic lattice.
 * https://oeis.org/A035713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 36 a(n) = Σ_{j=1}^{min(18,n)} C(18,j) * C(n-1,
 *            j-1) * 2^j (where C(k, r) = binomial coefficient)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 46, u128 n <= 636,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t C18[19] = {
    (seq_t)1, (seq_t)18, (seq_t)153, (seq_t)816, (seq_t)3060,
    (seq_t)8568, (seq_t)18564, (seq_t)31824, (seq_t)43758,
    (seq_t)48620, (seq_t)43758, (seq_t)31824, (seq_t)18564,
    (seq_t)8568, (seq_t)3060, (seq_t)816, (seq_t)153,
    (seq_t)18, (seq_t)1
};

static const seq_t POW2[19] = {
    (seq_t)1, (seq_t)2, (seq_t)4, (seq_t)8, (seq_t)16,
    (seq_t)32, (seq_t)64, (seq_t)128, (seq_t)256,
    (seq_t)512, (seq_t)1024, (seq_t)2048, (seq_t)4096,
    (seq_t)8192, (seq_t)16384, (seq_t)32768, (seq_t)65536,
    (seq_t)131072, (seq_t)262144
};

static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        /* res = res * (n - k + i) / i  (exact division) */
        res = (res * (seq_t)(n - k + i)) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)36;

    idx_t upper = (n < (idx_t)18) ? n : (idx_t)18;
    seq_t sum = (seq_t)0;
    for (idx_t j = (idx_t)1; j <= upper; ++j) {
        seq_t term = C18[j];
        term = term * binom((idx_t)(n - (idx_t)1), j - (idx_t)1);
        term = term * POW2[j];
        sum += term;
    }
    return sum;
}
