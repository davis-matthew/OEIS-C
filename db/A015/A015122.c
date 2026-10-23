/*
 * A015122  Inverse of 1113th cyclotomic polynomial.
 * https://oeis.org/A015122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A015122 – inverse of the 1113‑th cyclotomic polynomial.
    Formula: a(n) is the coefficient of x^n in the power series 1/Φ_{1113}(x).
    Φ_{1113}(x) has degree 624, so a(n) follows the recurrence
        a(0) = 1,
        a(n) = –∑_{k=1}^{624} p_k · a(n–k)  (n ≥ 1)
    where (p_k) are the coefficients of Φ_{1113}(x).  The sequence is
    periodic with period 1113.  The coefficients p_k are obtained by
    multiplying the integers (x^{1113/d} – 1)^{μ(d)} over all divisors d of 1113.
 */

#include <stddef.h>

#define DEG 624
#define PERIOD 1113

/* Divisors of 1113 and their Möbius values. */
static const idx_t divs[8] = {1, 3, 7, 21, 53, 159, 371, 1113};
static const int mu[8]   = { 1, -1, -1, 1, -1, 1, 1, -1 };

/* Periodic sequence values. */
static seq_t period[PERIOD] = {0};

/* Coefficients of Φ_{1113}(x). */
static seq_t coeff[DEG + 1] = {0};

static int initialized = 0;

seq_t a(idx_t n)
{
    if (!initialized)
    {
        /* 1. Compute Φ_{1113}(x) coefficients.  */
        /* start with polynomial 1. */
        for (size_t i = 0; i <= DEG; ++i) coeff[i] = 0;
        coeff[0] = 1;

        static seq_t temp[DEG + 1];
        for (int d = 0; d < 8; ++d)
        {
            idx_t k = 1113 / divs[d];
            if (mu[d] == 1)
            {
                /* multiply by (x^k – 1) : new[j] = old[j] – old[j–k]  */
                for (size_t j = DEG + 1; j-- > k; )
                    coeff[j] -= coeff[j - k];
                /* coefficients < k remain unchanged. */
            }
            else /* mu[d] == –1 : multiply by 1/(x^k – 1) = 1 + x^k + x^{2k} + … */
            {
                /* compute new coefficients = prefix sums over residues modulo k. */
                for (size_t j = 0; j <= DEG; ++j) temp[j] = 0;

                for (idx_t r = 0; r < k; ++r)
                {
                    seq_t sum = 0;
                    for (size_t j = r; j <= DEG; j += k)
                    {
                        sum += coeff[j];
                        temp[j] = sum;
                    }
                }
                for (size_t j = 0; j <= DEG; ++j) coeff[j] = temp[j];
            }
        }

        /* 2. Compute first PERIOD values of the inverse series. */
        period[0] = 1;
        for (size_t idx = 1; idx < PERIOD; ++idx)
        {
            seq_t s = 0;
            for (size_t k = 1; k <= DEG && k <= idx; ++k)
                s += coeff[k] * period[idx - k];
            period[idx] = -s;
        }

        initialized = 1;
    }

    return period[(size_t)(n % PERIOD)];
}
