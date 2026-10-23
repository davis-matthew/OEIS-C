/*
 * A014870  Inverse of 861st cyclotomic polynomial.
 * https://oeis.org/A014870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the power series expansion of
 *            1/Φ₈₆₁(x)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define PERIOD 861
#define N_MAX 913   /* 3 + 7 + 41 + 861 + 1 */
#define D_MAX 433   /* 1 + 21 + 123 + 287 + 1 */
#define Q_MAX 481   /* φ(861) + 1 */

static bool initialized = false;
static seq_t period[PERIOD];
static seq_t q_coef[Q_MAX];

/* multiply poly[0..len-1] by (x^d - 1) and store back in poly; update len */
static void mul_shift_minus_one(seq_t *poly, size_t *len, size_t d)
{
    size_t old_len = *len;
    size_t new_len = old_len + d;
    seq_t tmp[N_MAX];
    for (size_t i = 0; i < new_len; ++i) tmp[i] = 0;
    for (size_t i = 0; i < old_len; ++i) {
        tmp[i] -= poly[i];
        tmp[i + d] += poly[i];
    }
    for (size_t i = 0; i < new_len; ++i)
        poly[i] = tmp[i];
    *len = new_len;
}

/* exact division: numerator/denominator where both are polynomials with
   integer coef; leading coeff of denominator is 1 and division is exact. */
static void poly_div_exact(const seq_t *num, size_t len_n,
                           const seq_t *den, size_t len_d,
                           seq_t *div)
{
    seq_t rem[N_MAX];
    for (size_t i = 0; i < len_n; ++i) rem[i] = num[i];

    size_t q_len = len_n - len_d + 1;
    for (size_t i = 0; i < q_len; ++i) div[i] = 0;

    for (long long i = (long long)len_n - 1; i >= (long long)len_d - 1; --i) {
        seq_t coeff = rem[i];
        if (coeff != 0) {
            size_t pos = (size_t)(i - (long long)len_d + 1);
            div[pos] = coeff;
            for (size_t j = 0; j < len_d; ++j)
                rem[pos + j] -= coeff * den[j];
        }
    }
}

/* precompute Φ₈₆₁(x) coefficients and the 861‑term periodic sequence */
static void init_period(void)
{
    seq_t N[N_MAX] = {0};
    seq_t D[D_MAX] = {0};
    size_t n_len = 1, d_len = 1;
    N[0] = 1;  D[0] = 1;

    static const size_t num_factors[] = {3, 7, 41, 861};
    static const size_t den_factors[] = {1, 21, 123, 287};

    for (size_t idx = 0; idx < sizeof(num_factors)/sizeof(num_factors[0]); ++idx)
        mul_shift_minus_one(N, &n_len, num_factors[idx]);
    for (size_t idx = 0; idx < sizeof(den_factors)/sizeof(den_factors[0]); ++idx)
        mul_shift_minus_one(D, &d_len, den_factors[idx]);

    seq_t Q[Q_MAX] = {0};
    poly_div_exact(N, n_len, D, d_len, Q);

    for (size_t i = 0; i < Q_MAX; ++i) q_coef[i] = Q[i];
    q_coef[0] = 1;

    period[0] = 1;
    for (size_t n = 1; n < PERIOD; ++n) {
        seq_t sum = 0;
        size_t max_i = n < 480 ? n : 480;
        for (size_t i = 1; i <= max_i; ++i)
            sum -= q_coef[i] * period[n - i];
        period[n] = sum;
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init_period();
    return period[(size_t)(n % PERIOD)];
}
