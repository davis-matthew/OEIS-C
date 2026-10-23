/*
 * A015527  Inverse of 1518th cyclotomic polynomial.
 * https://oeis.org/A015527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in the power series expansion of 1 /
 *            Φ1518(x)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* static data for the periodic sequence */
static seq_t period[1518];
static seq_t poly[16][1520];   /* coefficient tables for Φ_d, d|1518 */
static int  deg[16];           /* degree of each Φ_d */
static uint64_t divs[16];      /* list of divisors of 1518, sorted */
static int dcnt;               /* number of divisors */

/* compute all divisors of 1518, sorted increasingly */
static void compute_divisors(void)
{
    const uint64_t N = 1518ull;
    dcnt = 0;
    for (uint64_t i = 1; i * i <= N; ++i) {
        if (N % i == 0) {
            divs[dcnt++] = i;
            if (i != N / i) divs[dcnt++] = N / i;
        }
    }
    /* simple bubble sort – at most 16 items */
    for (int i = 0; i < dcnt; ++i) {
        for (int j = i + 1; j < dcnt; ++j) {
            if (divs[j] < divs[i]) {
                uint64_t t = divs[i];
                divs[i] = divs[j];
                divs[j] = t;
            }
        }
    }
}

/* compute Φ_d(x) for every divisor d of 1518 and store in poly, deg */
static void compute_polynomials(void)
{
    /* clear coefficient tables */
    for (int i = 0; i < dcnt; ++i) {
        for (int j = 0; j <= 1518; ++j) poly[i][j] = 0;
        deg[i] = 0;
    }

    for (int idx = 0; idx < dcnt; ++idx) {
        uint64_t d = divs[idx];
        if (d == 1) {               /* Φ₁(x) = x – 1 */
            poly[idx][0] = -1;
            poly[idx][1] = 1;
            deg[idx] = 1;
            continue;
        }

        /* product B(x) = ∏ Φ_reference for all d' < d, d' | d */
        seq_t B[1520] = {0};
        seq_t tmp[1520] = {0};
        B[0] = 1;
        int degB = 0;

        for (int j = 0; j < idx; ++j) {
            if (d % divs[j] != 0) continue;   /* not a proper divisor */
            int newDeg = degB + deg[j];
            for (int k = 0; k <= newDeg; ++k) tmp[k] = 0;
            for (int a = 0; a <= degB; ++a) {
                for (int b = 0; b <= deg[j]; ++b) {
                    tmp[a + b] += B[a] * poly[j][b];
                }
            }
            degB = newDeg;
            for (int k = 0; k <= degB; ++k) B[k] = tmp[k];
        }

        /* dividend = x^d – 1 */
        seq_t dividend[1520];
        for (int k = 0; k <= (int)d; ++k) dividend[k] = 0;
        dividend[0] = -1;
        dividend[(int)d] = 1;

        /* divide dividend by B to get Φ_d(x) */
        seq_t quotient[1520];
        int degQ = (int)d - degB; /* φ(d) */
        for (int k = 0; k <= degQ; ++k) quotient[k] = 0;
        for (int k = (int)d; k >= degB; --k) {
            seq_t coeff = dividend[k] / B[degB];   /* B[degB] == 1 */
            quotient[k - degB] = coeff;
            for (int b = 0; b <= degB; ++b) {
                dividend[k - degB + b] -= coeff * B[b];
            }
        }

        /* store the results */
        deg[idx] = degQ;
        for (int k = 0; k <= degQ; ++k) poly[idx][k] = quotient[k];
    }
}

/* build the periodic sequence the first time it is needed */
static void build_sequence(void)
{
    compute_divisors();
    compute_polynomials();

    /* locate index of d = 1518 in divs[] */
    int idxN = -1;
    for (int i = 0; i < dcnt; ++i) {
        if (divs[i] == 1518) { idxN = i; break; }
    }

    /* recurrence: Φ_N(x) * A(x) = 1  ->  a[0]=1,
       a[n] = - Σ_{k=1}^{min(n,degN)} c[k] * a[n-k]  */
    period[0] = 1;
    seq_t *c = poly[idxN];
    int dphi = deg[idxN];

    for (int n = 1; n < 1518; ++n) {
        seq_t sum = 0;
        int maxk = (n < dphi) ? n : dphi;
        for (int k = 1; k <= maxk; ++k) sum += c[k] * period[n - k];
        period[n] = -sum;
    }
}

/* the required external function */
seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) {
        build_sequence();
        initialized = 1;
    }
    unsigned idx = (unsigned)(n % 1518);
    return period[idx];
}
