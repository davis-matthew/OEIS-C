/*
 * A015828  Inverse of 1819th cyclotomic polynomial.
 * https://oeis.org/A015828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) is the n-th coefficient of the power series of 1/Φ_1819(x),
 *            * the reciprocal of the 1819th cyclotomic polynomial. * The
 *            sequence is periodic with period 1819
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define PERIOD 1819
#define DEG_D 124
#define DEG_N 1820
#define DEG_Q (DEG_N - DEG_D)   /* 1696 */

static bool init_flag = false;
static seq_t cache[PERIOD];

static void compute_cache(void) {
    seq_t D[DEG_D + 1];
    seq_t Np[DEG_N + 1];
    seq_t R[DEG_N + 1];
    seq_t Q[DEG_Q + 1];

    /* D(x) = (x^107-1)(x^17-1) = x^124 - x^107 - x^17 + 1 */
    for (idx_t i = 0; i <= DEG_D; ++i) D[i] = 0;
    D[0]   = 1;
    D[17]  = -1;
    D[107] = -1;
    D[124] = 1;

    /* N(x) = (x^1819-1)(x-1) = x^1820 - x^1819 - x + 1 */
    for (idx_t i = 0; i <= DEG_N; ++i) Np[i] = 0;
    Np[0]    = 1;
    Np[1]    = -1;
    Np[1819] = -1;
    Np[1820] = 1;

    /* long division: Np / D (leading coefficient of D is 1) */
    for (idx_t i = 0; i <= DEG_N; ++i) R[i] = Np[i];
    for (int k = DEG_N; k >= DEG_D; --k) {
        seq_t q = R[k];
        Q[k - DEG_D] = q;
        if (q != 0) {
            for (int j = 0; j <= DEG_D; ++j) {
                R[k - DEG_D + j] -= q * D[j];
            }
        }
    }

    /* recurrence for the coefficients of 1/Φ_1819(x) */
    cache[0] = 1;
    for (idx_t n = 1; n < PERIOD; ++n) {
        seq_t sum = 0;
        idx_t maxk = (n <= DEG_Q) ? n : DEG_Q;
        for (idx_t k = 1; k <= maxk; ++k) {
            sum += Q[k] * cache[n - k];
        }
        cache[n] = -sum;
    }

    init_flag = true;
}

seq_t a(idx_t n) {
    if (!init_flag) compute_cache();
    return cache[n % PERIOD];
}
