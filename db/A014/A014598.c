/*
 * A014598  Inverse of 589th cyclotomic polynomial.
 * https://oeis.org/A014598
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

/* Implementation: inverse of the 589th cyclotomic polynomial.
 *  The sequence is periodic with period 589. */

#include <stdbool.h>

static seq_t tbl[589];
static bool tbl_inited = false;

static void init_seq(void)
{
    const int degN   = 590;
    const int degD   = 50;
    const int degPhi = 540;
    seq_t N[degN + 1];
    seq_t D[degD + 1];
    seq_t Q[degPhi + 1];
    seq_t a[589];

    /* Initialize work arrays */
    for (int i = 0; i <= degN; ++i)   N[i] = 0;
    for (int i = 0; i <= degD; ++i)   D[i] = 0;
    for (int i = 0; i <= degPhi; ++i) Q[i] = 0;

    /* numerator: (x^589 - 1)(x - 1) = x^590 - x^589 - x + 1 */
    N[0]   = 1;
    N[1]   = -1;   /* coefficient of x */
    N[589] = -1;   /* coefficient of x^589 */
    N[590] = 1;    /* coefficient of x^590 */

    /* denominator: (x^31 - 1)(x^19 - 1) = 1 - x^19 - x^31 + x^50 */
    D[0]  = 1;
    D[19] = -1;
    D[31] = -1;
    D[50] = 1;

    /* Long division: N(x) = D(x) * Q(x)  (D is monic of degree 50) */
    for (int i = degN; i >= degD; --i)
    {
        seq_t coef = N[i];
        if (coef != 0)
        {
            Q[i - degD] = coef;
            for (int j = 0; j <= degD; ++j)
            {
                N[(i - degD) + j] -= coef * D[j];
            }
        }
    }

    /* Compute first 589 coefficients of the inverse series 1/Φ_589(x) */
    a[0] = 1;
    for (int n = 1; n < 589; ++n)
    {
        seq_t sum = 0;
        int maxk = (n < degPhi) ? n : degPhi;
        for (int k = 1; k <= maxk; ++k)
        {
            sum += Q[k] * a[n - k];
        }
        a[n] = -sum;
    }

    /* Store in static table */
    for (int i = 0; i < 589; ++i)
        tbl[i] = a[i];
}

seq_t a(idx_t n)
{
    if (!tbl_inited)
    {
        init_seq();
        tbl_inited = true;
    }
    return tbl[n % 589];
}
