/*
 * A041488  Numerators of continued fraction convergents to sqrt(261).
 * https://oeis.org/A041488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 15, u64 n <= 33, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*      Numerators of convergents to sqrt(261)
 *
 *      Recurrence definition:
 *          a0 = 16                              (integer part of sqrt(261))
 *          Let (b1,b2,…,bL) be the periodic part of the continued fraction expansion
 *          of sqrt(261), e.g. b1=2, b2=5, … .
 *          For k ≥ 0
 *              a[k] = a0                    if k == 0
 *              a[k] = b[(k-1) mod L]       otherwise
 *          With p[-2] = 0, p[-1] = 1
 *              p[k] = a[k] * p[k-1] + p[k-2]
 *          The sequence a(n) is  a(n) = p[n]   (n ≥ 0).
 *
 *      The period (b1…bL) for sqrt(261) is:
 *          2, 5, 10, 19, 38, 74, 151, 303, 606, 1212, 2425, 4850, 9701,
 *          19402, 38804, 77609, 155218, 310439, 620879, 1241756, 2483757,
 *          4967514, 9935029, 19870058
 *
 *      (The period ends when a[k] = 2 * a0 = 32.)
 *
 *      Complexity:   O(n) time, O(1) extra space.
 *      Note:  All arithmetic is done in type `seq_t`, which supports
 *      arbitrary precision if required.
 */

#include <stdint.h>
#include <stdbool.h>

/* Periodic part of sqrt(261) continued fraction */
static int period_b[24];
static idx_t period_len = 0;
static bool period_computed = false;

/* Compute the continued‑fraction period of sqrt(261). */
static void compute_period(void)
{
    if (period_computed) return;

    const idx_t D = 261;
    const int a0 = 16;                /* floor(sqrt(261)) */
    idx_t m = 0, d = 1;
    int a = a0;
    int i = 0;

    while (true) {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        period_b[i++] = a;
        if (a == 2 * a0)              /* end of period */
            break;
    }
    period_len = i;
    period_computed = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    compute_period();

    /* Index 0 is the integer part itself */
    if (n == 0) {
        return (seq_t)16;
    }

    /* Compute convergents iteratively */
    seq_t p_prev2 = 0;   /* p[-2] */
    seq_t p_prev1 = 1;   /* p[-1] */
    seq_t p_curr  = 0;   /* current p[k] */

    for (idx_t k = 0; k <= n; ++k) {
        int ak;
        if (k == 0) {
            ak = 16;
        } else {
            /* periodic part repeats after the first term */
            ak = period_b[(k - 1) % period_len];
        }
        /* p[k] = ak * p_prev1 + p_prev2 */
        p_curr = (seq_t)ak * p_prev1 + p_prev2;

        /* shift for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }
    return p_curr;       /* p[n] is the desired numerator */
}
