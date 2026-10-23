/*
 * A042817  Denominators of continued fraction convergents to sqrt(939).
 * https://oeis.org/A042817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A042817
 *  Denominators of continued fraction convergents to sqrt(939)
 *
 *  Recurrence:
 *      Q_-2 = 1          // denominator for index -2
 *      Q_-1 = 0          // denominator for index -1
 *      Q_n  = a_n * Q_{n-1} + Q_{n-2}   for n >= 0
 *
 *  Continued‑fraction coefficients for sqrt(939):
 *      a_0 = 30
 *      a_k (k >= 1) repeats the block
 *          [1, 1, 1, 4, 20, 4, 1, 1, 1, 60]    (period length 10)
 *
 *  a(n) is Q_n from the above recurrence. */

#include <stdlib.h>
#include <stddef.h>

static const seq_t PERIOD_COEFFS[] = {1,1,1,4,20,4,1,1,1,60};
static const size_t PERIOD_LEN = sizeof(PERIOD_COEFFS)/sizeof(PERIOD_COEFFS[0]);

seq_t a(idx_t n)
{
    /* static cache for denominators */
    static seq_t *denom   = NULL;  /* array of Q_0 … Q_{size-1} */
    static size_t  cap    = 0;      /* allocated capacity of denom */
    static idx_t   size   = 0;      /* number of cached denominators */

    /* values for recurrence */
    static seq_t prev2 = 1; /* Q_{-2} */
    static seq_t prev1 = 0; /* Q_{-1} */

    /* Ensure we have a cache space and compute up to n */
    if (n >= size) {
        /* Allocate or grow cache */
        if (denom == NULL) {
            cap = 16;
            denom = (seq_t*)malloc(cap * sizeof(seq_t));
        }
        while (cap <= (size_t)n) { /* ensure cap > n */
            cap <<= 1;
            denom = (seq_t*)realloc(denom, cap * sizeof(seq_t));
        }

        /* Compute missing terms */
        while (size <= n) {
            seq_t coeff;
            if (size == 0)          /* a_0 */
                coeff = (seq_t)30;   /* floor(sqrt(939)) = 30 */
            else                    /* periodic part */
                coeff = PERIOD_COEFFS[(size - 1) % PERIOD_LEN];

            seq_t cur = coeff * prev1 + prev2;
            denom[size] = cur;

            /* shift recurrence values for next step */
            prev2 = prev1;
            prev1 = cur;
            ++size;
        }
    }

    /* Return cached value */
    return denom[n];
}
