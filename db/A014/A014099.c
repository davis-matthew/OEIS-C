/*
 * A014099  Inverse of 90th cyclotomic polynomial.
 * https://oeis.org/A014099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Inverse of 90th cyclotomic polynomial Φ_90(x).
 * The sequence is periodic of period 90.
 * It satisfies a(0) = 1 and, for n > 0,
 *     a(n) = -Σ_{i=1}^{24} c[i] * a(n-i),
 * where c[i] are the coefficients of Φ_90(x) (degree 24).
 */

#include <stddef.h>
#include <stdbool.h>

#define NUM_DEG 4          /* number of factors with exponent +1 */
#define DEN_DEG 4          /* number of factors with exponent -1 */
#define MAX_N_DEG 120
#define MAX_D_DEG 96
#define PHI_DEG 24
#define PERIOD 90

static bool init = false;
static seq_t phi[PHI_DEG + 1];
static seq_t seq_tbl[PERIOD];

/* Compute the coefficients of Φ_90(x) via the product formula */
static void compute_phi90(void)
{
    const int num_factors[NUM_DEG] = {6, 9, 15, 90};
    const int den_factors[DEN_DEG] = {3, 18, 30, 45};

    seq_t N[MAX_N_DEG + 1] = {0};
    seq_t tmpN[MAX_N_DEG + 1];
    int degN = 0;
    N[0] = 1;            /* start with 1 */

    for (int k = 0; k < NUM_DEG; ++k) {
        int d = num_factors[k];
        int newdeg = degN + d;
        for (int i = 0; i <= MAX_N_DEG; ++i) tmpN[i] = 0;
        for (int i = 0; i <= degN; ++i) {
            tmpN[i]     -= N[i];     /* constant term is subtracted */
            tmpN[i + d] += N[i];     /* x^d term is added */
        }
        for (int i = 0; i <= newdeg; ++i) N[i] = tmpN[i];
        degN = newdeg;
    }

    seq_t D[MAX_D_DEG + 1] = {0};
    seq_t tmpD[MAX_D_DEG + 1];
    int degD = 0;
    D[0] = 1;

    for (int k = 0; k < DEN_DEG; ++k) {
        int d = den_factors[k];
        int newdeg = degD + d;
        for (int i = 0; i <= MAX_D_DEG; ++i) tmpD[i] = 0;
        for (int i = 0; i <= degD; ++i) {
            tmpD[i]     -= D[i];
            tmpD[i + d] += D[i];
        }
        for (int i = 0; i <= newdeg; ++i) D[i] = tmpD[i];
        degD = newdeg;
    }

    /* Long division: N(x) = D(x) * Φ(x)  (φ[0] == 1)  */
    for (int k = 0; k <= PHI_DEG; ++k) {
        seq_t sum = N[k];
        int i_max = (k < degD) ? k : degD;
        for (int i = 1; i <= i_max; ++i) {
            sum -= D[i] * phi[k - i];
        }
        phi[k] = sum;
    }
}

/* Compute the first period of the inverse series */
static void compute_seq_table(void)
{
    seq_tbl[0] = 1;
    for (idx_t n = 1; n < PERIOD; ++n) {
        seq_t sum = 0;
        idx_t imax = (n < PHI_DEG) ? n : PHI_DEG;
        for (idx_t i = 1; i <= imax; ++i) {
            sum += phi[i] * seq_tbl[n - i];
        }
        seq_tbl[n] = -sum;
    }
}

seq_t a(idx_t n)
{
    if (!init) {
        compute_phi90();
        compute_seq_table();
        init = true;
    }
    return seq_tbl[(size_t)(n % PERIOD)];
}
