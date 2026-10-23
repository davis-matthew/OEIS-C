/*
 * A162513  G.f. is the polynomial (1-x^3) * (1-x^6) * (1-x^9) * (1-x^12) *
 *          (1-x^15) / (1-x)^5.
 * https://oeis.org/A162513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in
 *            (1-x^3)(1-x^6)(1-x^9)(1-x^12)(1-x^15)/(1-x)^5 (Only finitely
 *            many non‑zero terms, all a(n) for n>40 are zero.)
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool initialized = false;
static long long Ncoeff[46];      /* coefficients of numerator polynomial */
static seq_t Acoeff[41];          /* coefficients of the sequence */

/* pre‑compute Ncoeff and Acoeff once */
static void init_sequence(void)
{
    /* compute coefficients of N(x) = ∏_{k=1}^5 (1 - x^{3k}) */
    for (int i = 0; i <= 45; ++i) Ncoeff[i] = 0;
    Ncoeff[0] = 1;
    for (int m = 1; m <= 5; ++m) {
        int e = 3 * m;
        for (int i = 45 - e; i >= 0; --i) {
            Ncoeff[i + e] -= Ncoeff[i];
        }
    }

    /* binomial coefficients C(5,k) */
    static const int binom5[6] = {1, 5, 10, 10, 5, 1};

    /* compute sequence via recurrence
       (1-x)^5 * A(x) = N(x)  =>  a(n) = N(n) - Σ_{k=1}^5 (-1)^k * C(5,k) * a(n-k)  */
    for (int n = 0; n <= 40; ++n) {
        long long sum = 0;
        for (int k = 1; k <= 5 && n >= k; ++k) {
            int coeff = (k & 1) ? -binom5[k] : binom5[k];
            sum += coeff * (long long)Acoeff[n - k];
        }
        long long val = Ncoeff[n] - sum;
        if (val < 0) val = 0;            /* safety: sequence values are non‑negative */
        Acoeff[n] = (seq_t)val;
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init_sequence();
    if (n > 40) return 0;              /* all terms beyond index 40 are zero */
    return Acoeff[(int)n];
}
