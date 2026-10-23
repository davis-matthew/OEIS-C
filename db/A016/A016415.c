/*
 * A016415  462nd cyclotomic polynomial.
 * https://oeis.org/A016415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 n <= 120, i16 n <= 120, i32 n <= 120, i64 n <= 120,
 *            i128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the coefficient of x^n in the 462nd cyclotomic polynomial Φ₄₆₂(x)
   by constructing the cyclotomic polynomials for all divisors of 462 via
   the recursive formula Φ_d(x) = (x^d − 1) / ∏_{m|d, m<d} Φ_m(x). */

#include <stddef.h>
#include <stdbool.h>

/* Constants ------------------------------------------------------------ */
#define MAX_DIV 16          /* number of divisors of 462 */
#define MAX_DEG 120         /* φ(462) = 120 */
#define K_POLY 463          /* maximum degree needed for intermediate polynomials (462+1) */

/* Divisors of 462 in ascending order ------------------------------ */
static const idx_t divisors[MAX_DIV] = {
    1ULL, 2ULL, 3ULL, 6ULL, 7ULL, 11ULL, 14ULL, 21ULL,
    22ULL, 33ULL, 42ULL, 66ULL, 77ULL, 154ULL, 231ULL, 462ULL
};

/* Storage for each Φ_d polynomial (monic, degree ≤ 120) ---------------- */
static seq_t poly[MAX_DIV][MAX_DEG + 1];
static size_t degree[MAX_DIV] = {0};

/* One‑time initialization flag --------------------------------- */
static bool initialized = false;

/* Exact division of two monic polynomials.
   num   – dividend array [0..deg_num]
   den   – divisor array [0..deg_den] (leading coefficient 1)
   quo   – quotient array [0..deg_quo]
   deg_quo out parameter for quotient degree            */
static void
div_exact(const seq_t *num, size_t deg_num,
          const seq_t *den, size_t deg_den,
          seq_t *quo, size_t *deg_quo)
{
    /* work array to hold the current dividend during division */
    static seq_t work[K_POLY];
    for (size_t i = 0; i <= deg_num; ++i) work[i] = num[i];

    *deg_quo = deg_num - deg_den;
    /* initialise quotient */
    for (size_t i = 0; i <= *deg_quo; ++i) quo[i] = 0;

    /* perform long division (den is monic) */
    for (int k = (int)deg_num; k >= (int)deg_den; --k) {
        seq_t coeff = work[k];                     /* denominator's leading coefficient is 1 */
        quo[k - (int)deg_den] = coeff;
        for (size_t i = 0; i <= deg_den; ++i) {
            work[k - (int)deg_den + i] -= coeff * den[i];
        }
    }
    /* any remaining work[k] for k < deg_den should be 0 if division is exact */
}

/* Compute all Φ_d for d | 462 -------------------------------------- */
static void
init_coeffs(void)
{
    if (initialized) return;

    for (size_t idx = 0; idx < MAX_DIV; ++idx) {
        idx_t d = divisors[idx];

        if (d == 1) {                       /* Φ₁(x) = x − 1 */
            degree[idx] = 1;
            poly[idx][0] = - (seq_t)1;
            poly[idx][1] =  (seq_t)1;
            continue;
        }

        /* Temporary arrays for intermediate polynomials ----------------- */
        static seq_t cur[K_POLY];        /* current polynomial, starts as x^d − 1 */
        static seq_t q[K_POLY];          /* quotient after each division step */

        /* initialise cur = x^d − 1 */
        for (size_t i = 0; i <= (size_t)d; ++i) cur[i] = 0;
        cur[0] = - (seq_t)1;
        cur[d] =  (seq_t)1;
        size_t deg_cur = d;

        /* Divide by Φ_m for each proper divisor m of d (already computed) */
        for (size_t j = 0; j < idx; ++j) {
            idx_t m = divisors[j];
            if (d % m != 0) continue;                /* not a divisor of d */
            /* divide cur (degree deg_cur) by Φ_m (degree degree[j]) */
            div_exact(cur, deg_cur,
                      poly[j], degree[j],
                      q, &deg_cur);
            /* copy quotient back to cur for next iteration */
            for (size_t i = 0; i <= deg_cur; ++i) cur[i] = q[i];
        }

        /* Store Φ_d (degree <= MAX_DEG) */
        degree[idx] = deg_cur;
        for (size_t i = 0; i <= deg_cur; ++i) poly[idx][i] = cur[i];
        for (size_t i = deg_cur + 1; i <= MAX_DEG; ++i) poly[idx][i] = 0;
    }

    initialized = true;
}

/* Public coefficient function --------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) init_coeffs();

    /* index of divisor 462 is the last one in the array */
    const size_t idx = MAX_DIV - 1;

    if ((idx_t)degree[idx] < n) return 0;
    return poly[idx][(size_t)n];
}
