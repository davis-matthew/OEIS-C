/*
 * A014264  Inverse of 255th cyclotomic polynomial.
 * https://oeis.org/A014264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the power series expansion of
 *            1/Φ255(x), the reciprocal of the 255th cyclotomic polynomial.
 *            The sequence is periodic with period 255
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define NUM_DEG_MAX 280   /* 255 + 17 + 5 + 3 */
#define DEN_DEG_MAX 152   /* 85 + 51 + 15 + 1 */
#define POLY_DEG    128   /* φ(255) = 128 */
#define PERIOD_LEN  255

/* Multiply the polynomial p by (x^k – 1) in place.
   deg points to its current degree (≤ max_deg – k). */
static void apply_factor_ll(long long p[], int *deg, int k, int max_deg)
{
    int new_deg = *deg + k;
    long long tmp[NUM_DEG_MAX + 1] = {0};
    for (int i = 0; i <= new_deg; ++i)
        tmp[i] = 0;
    for (int i = 0; i <= *deg; ++i) {
        tmp[i]     -= p[i];   /* –p[i]   */
        tmp[i + k] += p[i];   /* +p[i]   */
    }
    for (int i = 0; i <= new_deg; ++i)
        p[i] = tmp[i];
    *deg = new_deg;
}

/* Return the nth term of A014264. */
seq_t a(idx_t n)
{
    static seq_t coeff[POLY_DEG + 1];   /* coefficients of Φ255(x)   */
    static seq_t period[PERIOD_LEN];   /* one period of the inverse  */
    static int initialized = 0;

    if (!initialized)
    {
        /* ----- Build Φ255(x) as (x^255-1)(x^17-1)(x^5-1)(x^3-1) /
            (x^85-1)(x^51-1)(x^15-1)(x-1)  ----- */
        long long num[NUM_DEG_MAX + 1] = {0};
        long long den[DEN_DEG_MAX + 1] = {0};
        int deg_num = 0, deg_den = 0;
        num[0] = den[0] = 1;

        /* Apply numerator factors. */
        const int numer_k[4] = {255, 17, 5, 3};
        for (int i = 0; i < 4; ++i)
            apply_factor_ll(num, &deg_num, numer_k[i], NUM_DEG_MAX);

        /* Apply denominator factors. */
        const int denom_k[4] = {85, 51, 15, 1};
        for (int i = 0; i < 4; ++i)
            apply_factor_ll(den, &deg_den, denom_k[i], DEN_DEG_MAX);

        /* Long division: num / den to obtain Φ255(x). */
        long long rem[NUM_DEG_MAX + 1];
        long long quo[POLY_DEG + 1] = {0};
        for (int i = 0; i <= NUM_DEG_MAX; ++i)
            rem[i] = num[i];

        for (int i = NUM_DEG_MAX; i >= deg_den; --i) {
            long long coeff_q = rem[i];
            if (coeff_q) {
                int pos = i - deg_den;
                quo[pos] = coeff_q;
                for (int j = 0; j <= deg_den; ++j)
                    rem[pos + j] -= coeff_q * den[j];
            }
        }

        for (int i = 0; i <= POLY_DEG; ++i)
            coeff[i] = (seq_t)quo[i];

        /* ----- Compute one period of the inverse series ----- */
        period[0] = 1;
        for (idx_t i = 1; i < PERIOD_LEN; ++i) {
            seq_t sum = 0;
            int kmax = (i <= (idx_t)POLY_DEG) ? (int)i : POLY_DEG;
            for (int k = 1; k <= kmax; ++k)
                sum += coeff[k] * period[i - k];
            period[i] = -sum;
        }
        initialized = 1;
    }

    return period[n % PERIOD_LEN];
}
