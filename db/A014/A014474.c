/*
 * A014474  Inverse of 465th cyclotomic polynomial.
 * https://oeis.org/A014474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in 1/Φ_465(x), where Φ_465 is the
 *            465‑th cyclotomic polynomial. The coefficients form a periodic
 *            sequence of period 465
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define PERIOD 465
#define MAXLEN 600  /* safe upper bound for intermediate polynomial degrees */

static seq_t period[PERIOD];
static bool ready = false;

/* Compute Φ_465(x) and the first PERIOD coefficients of its reciprocal. */
static void compute_sequence(void)
{
    /* Divisors of 465 and corresponding Möbius values μ(465/d). */
    const int divs[] = {1, 3, 5, 15, 31, 93, 155, 465};
    const int mus[]  = {-1, 1, 1, -1, 1, -1, -1, 1};
    const int ndiv = 8;

    /* ---------- Numerator: product of (x^d – 1) for μ = +1 ----------
       ---------------------------------------------------------- */
    seq_t num[MAXLEN] = {0};
    int deg_num = 0;
    num[0] = 1;                               /* start with 1 */

    for (int idx = 0; idx < ndiv; ++idx) {
        if (mus[idx] != 1) continue;          /* skip μ = –1 */
        int d = divs[idx];
        seq_t tmp[MAXLEN] = {0};
        for (int i = 0; i <= deg_num; ++i) {
            tmp[i]     += -num[i];
            tmp[i + d] +=  num[i];
        }
        deg_num += d;
        for (int i = 0; i <= deg_num; ++i) num[i] = tmp[i];
    }

    /* ---------- Denominator: product of (x^d – 1) for μ = –1 ----------
       ----------------------------------------------------------- */
    seq_t den[MAXLEN] = {0};
    int deg_den = 0;
    den[0] = 1;                               /* start with 1 */

    for (int idx = 0; idx < ndiv; ++idx) {
        if (mus[idx] != -1) continue;         /* skip μ = +1 */
        int d = divs[idx];
        seq_t tmp[MAXLEN] = {0};
        for (int i = 0; i <= deg_den; ++i) {
            tmp[i]     += -den[i];
            tmp[i + d] +=  den[i];
        }
        deg_den += d;
        for (int i = 0; i <= deg_den; ++i) den[i] = tmp[i];
    }

    /* ---------- Divide numerator by denominator ----------
       ------------------------------------------------- */
    int deg_phi = deg_num - deg_den;           /* should be 240 */
    seq_t quot[MAXLEN] = {0};
    seq_t rem[MAXLEN]  = {0};

    for (int i = 0; i <= deg_num; ++i) rem[i] = num[i];

    for (int i = deg_num; i >= deg_den; --i) {
        seq_t q = rem[i];
        if (q != 0) {
            int shift = i - deg_den;
            quot[shift] = q;
            for (int j = 0; j <= deg_den; ++j) {
                rem[shift + j] -= q * den[j];
            }
        }
    }

    /* ---------- Coefficients of Φ_465(x) ----------
       ------------------------------------------- */
    const int DEG = deg_phi;                      /* 240 */
    seq_t phi_coeff[DEG + 1];
    for (int i = 0; i <= DEG; ++i) phi_coeff[i] = quot[i];

    /* ---------- Compute the first PERIOD coefficients of 1/Φ_465(x) ----------
       --------------------------------------------------------------- */
    seq_t seq[PERIOD] = {0};
    seq[0] = 1;

    for (int m = 1; m < PERIOD; ++m) {
        seq_t sum = 0;
        int max_i = (m < DEG) ? m : DEG;
        for (int i = 1; i <= max_i; ++i) {
            sum += phi_coeff[i] * seq[m - i];
        }
        seq[m] = -sum;
    }

    /* ---------- Store result ----------
       --------------------------------- */
    for (int i = 0; i < PERIOD; ++i) period[i] = seq[i];
    ready = true;
}

seq_t a(idx_t n)
{
    if (!ready) compute_sequence();
    return period[n % PERIOD];
}
