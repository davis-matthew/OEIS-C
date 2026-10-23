/*
 * A162411  Number of reduced words of length n in the Weyl group D_42.
 * https://oeis.org/A162411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   The generating function for reduced words in the Weyl group *
 *            D_42 is * * G(x) = (1 - x^42) * ∏_{i=1}^{41} (1 - x^{2i}) / (1 -
 *            x)^42 * * The coefficient of x^n in G(x) gives a(n). Because the
 *            numerator has * a factor (1-x)^42, G(x) is actually a polynomial
 *            of degree * 42*41 = 1722, so a(n) = 0 fo...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   binom, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binom.h"
#include "../utils/seq_mul.h"

#define M          42
#define MAX_DEG   (M * M)          /* degree of the numerator polynomial */
#define MAX_LEN   (M * (M - 1))    /* maximal n with non‑zero a(n) */

/* static storage for the dynamic programming */
static seq_t E[MAX_DEG + 1];
static seq_t O[MAX_DEG + 1];
static seq_t newE[MAX_DEG + 1];
static seq_t newO[MAX_DEG + 1];
static seq_t binomVals[MAX_LEN + 1];
static int initialized = 0;

/* helper: zero an array of seq_t */
static void zero_array(seq_t *arr, size_t sz)
{
    for (size_t i = 0; i < sz; ++i)
        arr[i] = 0;
}

/* initialise the tables once */
static void init(void)
{
    size_t i;

    zero_array(E, MAX_DEG + 1);
    zero_array(O, MAX_DEG + 1);
    E[0] = 1;  /* empty subset has even parity */

    /* factors: first 42, then 2,4,…,82 */
    const int factors[] = {42,
        2, 4, 6, 8, 10, 12, 14, 16, 18, 20,
        22, 24, 26, 28, 30, 32, 34, 36, 38,
        40, 42, 44, 46, 48, 50, 52, 54, 56,
        58, 60, 62, 64, 66, 68, 70, 72, 74,
        76, 78, 80, 82};

    const int num_factors = sizeof(factors) / sizeof(factors[0]);

    for (int f = 0; f < num_factors; ++f) {
        int p = factors[f];
        zero_array(newE, MAX_DEG + 1);
        zero_array(newO, MAX_DEG + 1);

        for (i = 0; i <= MAX_DEG; ++i) {
            seq_t e = E[i];
            seq_t o = O[i];

            /* exclude the factor (choose 1) */
            newE[i] += e;
            newO[i] += o;

            /* include the factor (-x^p) */
            if (i + (size_t)p <= MAX_DEG) {
                newE[i + p] += o;   /* sign flips: odd -> even */
                newO[i + p] += e;   /* even -> odd   */
            }
        }

        /* swap E<->newE, O<->newO */
        for (i = 0; i <= MAX_DEG; ++i) {
            E[i] = newE[i];
            O[i] = newO[i];
        }
    }

    /* pre‑compute the binomial coefficients needed for the convolution */
    for (idx_t t = 0; t <= MAX_LEN; ++t) {
        binomVals[t] = binom((idx_t)(M + t - 1), t);
    }

    initialized = 1;
}

seq_t a(idx_t n)
{
    if (!initialized) init();

    if (n > (idx_t)MAX_LEN)
        return 0;

    seq_t sumPos = 0;
    seq_t sumNeg = 0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t bin = binomVals[n - k];
        sumPos += seq_mul(E[k], bin);
        sumNeg += seq_mul(O[k], bin);
    }

    return sumPos - sumNeg;  /* sumPos >= sumNeg for all n due to definition */
}
