/*
 * A015752  Inverse of 1743rd cyclotomic polynomial.
 * https://oeis.org/A015752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) = -∑_{i=1}^{984} coeff[i] * a(n-i), where coeff are the
 *            coefficients of the 1743‑rd cyclotomic polynomial (φ(1743)=984).
 *            The sequence is periodic with period 1743
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define PERIOD     1743
#define MAX_POLY   2000   /* > 1836 (max degree of intermediate polynomials) */

/* temporary storage for polynomial coefficients */
static seq_t poly_pos[MAX_POLY];
static seq_t poly_neg[MAX_POLY];
static size_t len_pos  = 0;
static size_t len_neg  = 0;
static seq_t seq_vals[PERIOD];
static bool initialized = false;

/* multiply \"poly\" in place by (x^d-1) : poly(x) -> poly(x)*(x^d-1) */
static void poly_mul_shift_inplace(seq_t *poly, size_t *len, size_t d)
{
    seq_t tmp[MAX_POLY] = {0};
    size_t new_len = *len + d;

    for (size_t i = 0; i < *len; ++i) {
        tmp[i]     -= poly[i];  /*   −poly[i]          */
        tmp[i + d] += poly[i];  /*   +poly[i]·x^d      */
    }
    for (size_t i = 0; i < new_len; ++i)
        poly[i] = tmp[i];
    *len = new_len;
}

/* divide \"num\" (length num_len) by \"den\" (length den_len),
   assuming division is exact.  Result is written to quotient.        */
static void poly_div(const seq_t *num, size_t num_len,
                     const seq_t *den, size_t den_len,
                     seq_t *quotient, size_t *q_len)
{
    size_t ndeg = num_len - 1;
    size_t ddeg = den_len - 1;
    *q_len = ndeg - ddeg + 1;

    seq_t rem[MAX_POLY];
    for (size_t i = 0; i < num_len; ++i)
        rem[i] = num[i];

    for (long long i = (long long)ndeg; i >= (long long)ddeg; --i) {
        seq_t coeff = rem[i];                /* leading term of remainder */
        quotient[i - ddeg] = coeff;

        /* subtract coeff * den(x) * x^(i-ddeg) from remainder */
        for (size_t j = 0; j < den_len; ++j) {
            rem[(size_t)(i - ddeg) + j] -= coeff * den[j];
        }
    }
}

/* initialise polynomial coefficients of Φ1743 and the sequence values */
static void init(void)
{
    if (initialized) return;
    initialized = true;

    /* build product of (x^m-1) for μ(d)=+1 (positive factors) */
    poly_pos[0] = 1;
    len_pos     = 1;
    /* m values corresponding to μ=+1: 1743, 83, 7, 3 */
    static const size_t pos_m[] = {1743, 83, 7, 3};
    for (size_t k = 0; k < sizeof(pos_m)/sizeof(pos_m[0]); ++k)
        poly_mul_shift_inplace(poly_pos, &len_pos, pos_m[k]);

    /* build product of (x^m-1) for μ(d)=-1 (negative factors) */
    poly_neg[0] = 1;
    len_neg     = 1;
    /* m values corresponding to μ=-1: 581, 249, 21, 1 */
    static const size_t neg_m[] = {581, 249, 21, 1};
    for (size_t k = 0; k < sizeof(neg_m)/sizeof(neg_m[0]); ++k)
        poly_mul_shift_inplace(poly_neg, &len_neg, neg_m[k]);

    /* Φ1743(x) = poly_pos / poly_neg */
    seq_t tmp_coeff[MAX_POLY];
    size_t tmp_len;
    poly_div(poly_pos, len_pos, poly_neg, len_neg, tmp_coeff, &tmp_len);

    /* copy coefficients to global poly array */
    static seq_t poly[MAX_POLY];
    for (size_t i = 0; i < tmp_len; ++i)
        poly[i] = tmp_coeff[i];
    size_t deg_phi = tmp_len - 1;   /* φ(1743) = 984 */

    /* compute first PERIOD terms from the recurrence */
    seq_vals[0] = 1;
    for (size_t n = 1; n < PERIOD; ++n) {
        seq_t val = 0;
        size_t lim = (n <= deg_phi) ? n : deg_phi;
        for (size_t i = 1; i <= lim; ++i)
            val -= poly[i] * seq_vals[n - i];
        seq_vals[n] = val;
    }
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!initialized) init();
    return seq_vals[(size_t)(n % PERIOD)];
}
