/*
 * A172106  The triangle T_2(n, m), where T_2(n, m) is the number of
 *          surjective multi-valued functions from {1, 1, 2, 3, ..., n-1} to
 *          {1, 2, 3, ..., m} by rows (n >= 1, 1 <= m <= n).
 * https://oeis.org/A172106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T_2(r, c) where r and c are obtained from the * flattened
 *            triangle of A172106. T_2(r, c) = Σ_{j=1..c} * binomial(c,j) *
 *            binomial(j+1,2) * (-1)^(c-j) * j^(r-2) (r≥2), * with T_2(1,1)=0
 * Range:     u8 n <= 17, u16 n <= 32, u32 n <= 84, u64 n <= 200,
 *            u128 n <= 514, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* local helpers – no external dependencies */
static seq_t binom_local(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;                /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - i + 1) / (seq_t)i;
    }
    return res;
}

static seq_t pow_uint_local(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t b = base;
    while (exp) {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

/* Return the first index of row r in the flattened triangle.
 * For r=1 result is 1, for r=2 result is 2, etc. */
static idx_t offset_row(idx_t r)
{
    return (r * (r - 1) / 2) + 1;
}

/* Main sequence generator */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out‑of‑range guard */

    /* find row containing n by increasing r until offset_row(r+1) > n */
    idx_t r = 1;
    while (offset_row(r + 1) <= n) ++r;

    /* column index within the row (1‑based) */
    idx_t c = n - offset_row(r) + 1;

    if (r == 1) return (seq_t)0;          /* first row is all zeros */

    seq_t sum = (seq_t)0;
    idx_t power_exponent = r - 2;

    for (idx_t j = 1; j <= c; ++j) {
        /* binomial(c, j) */
        seq_t bcj = binom_local(c, j);

        /* binomial(j+1, 2) = (j+1)*j/2 */
        seq_t bj1_2 = ((seq_t)(j + 1) * (seq_t)j) / 2;

        /* j^(r-2) */
        seq_t jp = pow_uint_local((seq_t)j, power_exponent);

        /* term = bcj * bj1_2 * jp */
        seq_t term = bcj * bj1_2 * jp;

        /* sign = (-1)^(c-j) */
        if (((c - j) & 1) == 0) {
            sum += term;
        } else {
            sum -= term;
        }
    }
    return sum;
}
