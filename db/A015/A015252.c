/*
 * A015252  Inverse of 1243rd cyclotomic polynomial.
 * https://oeis.org/A015252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   The coefficients a(n) of the power series 1/Φ₁₂₄₃(x) satisfy
 *            a(0)=1; for 1 ≤ n < 1243: a(n) = a(n-1) + q(n) where q(0)= 1,
 *            q(11)=q(113)=-1, q(124)= 1, and all other q(n)=0. The sequence
 *            is periodic with period 1243
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static bool ready = false;
static seq_t period[1243];   /* period[0] .. period[1242] */

/* initialize the periodic array once */
static void init_period(void)
{
    if (ready) return;

    period[0] = 1;    /* a(0) = 1 */
    for (int i = 1; i < 1243; ++i)
    {
        seq_t q = 0;
        if (i == 11)          q = (seq_t)-1;
        else if (i == 113)    q = (seq_t)-1;
        else if (i == 124)    q = (seq_t)1;
        period[i] = period[i - 1] + q;
    }
    ready = true;
}

/* external linkage as required */
seq_t a(idx_t n)
{
    init_period();
    return period[(size_t)(n % 1243)];
}
