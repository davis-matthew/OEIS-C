/*
 * A016383  323rd cyclotomic polynomial.
 * https://oeis.org/A016383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 n <= 288, i16 n <= 288, i32 n <= 288, i64 n <= 288,
 *            i128 n <= 288, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Sequence: coefficients of Φ_323(x).  The 323‑rd cyclotomic
 * polynomial Φ_323(x) = (x–1)(x^323–1)/[(x^17–1)(x^19–1)].
 * It has degree φ(323)=288 and its coefficients are all in {-1,0,1}. */

/* Global static cache for the 289 coefficients (indices 0..288).  */
static seq_t coeff_cache[289];

/* Compute the coefficients of Φ_323(x) and store them in coeff_cache.
 * This function is called only once. */
static void compute_phi_323(void)
{
    /* Numerator = (x–1)(x^323–1).  Degree 324. */
    seq_t num[325];
    for (size_t i = 0; i <= 324; ++i) num[i] = (seq_t)0;
    num[0] = (seq_t)1;         /* +1                      */
    num[1] = (seq_t)-1;        /* –x                      */
    num[323] = (seq_t)-1;      /* –x^323                  */
    num[324] = (seq_t)1;       /* +x^324                  */

    /* Denominator = (x^17–1)(x^19–1).  Degree 36. */
    seq_t den[37];
    for (size_t i = 0; i <= 36; ++i) den[i] = (seq_t)0;
    den[0] = (seq_t)1;
    den[17] = (seq_t)-1;
    den[19] = (seq_t)-1;
    den[36] = (seq_t)1;

    /* Quotient polynomial Q of degree 288 (indices 0..288). */
    seq_t Q[289] = {0};

    /* Long division: num / den. */
    for (int i = 324; i >= 36; --i)
    {
        seq_t c = num[i];
        if (c != (seq_t)0)
        {
            size_t qpos = (size_t)(i - 36);   /* x^qpos */
            Q[qpos] = c;
            /* subtract c * den shifted by qpos from num. */
            for (size_t j = 0; j <= 36; ++j)
            {
                num[qpos + j] -= c * den[j];
            }
        }
    }

    /* Store the results in the global cache. */
    for (size_t i = 0; i < 289; ++i)
    {
        coeff_cache[i] = Q[i];
    }
}

seq_t a(idx_t n)
{
    static bool ready = false;
    if (!ready)
    {
        compute_phi_323();
        ready = true;
    }

    /* Bounds check: coefficient exists for 0 ≤ n ≤ 288. */
    if (n > 288)
        return (seq_t)0;
    return coeff_cache[(size_t)n];
}
