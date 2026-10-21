/*
 * A233392  List of y coordinates of infinite family of points on the Mordell
 *          curve x^3-y^2=d over quadratic field Sqrt[13].
 * https://oeis.org/A233392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*(-1 + t[n] + t[n]^2) * sqrt(13*(26 + 10*t[n] +
 *            t[n]^2)) (OEIS A233392) with t[1] = 13, t[2] = 23377, t[3] =
 *            30349813, and for n ≥ 4 t[n] = 1299*t[n-1] - 1299*t[n-2] +
 *            t[n-3] (linear recurrence)
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute integer square root – floor(sqrt(x)) */
static seq_t seq_isqrt(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    while (low <= high)
    {
        seq_t mid = (low + high) / (seq_t)2;
        seq_t mid2 = mid * mid;
        if (mid2 == x)
            return mid;
        if (mid2 < x)
            low = mid + (seq_t)1;
        else
            high = mid - (seq_t)1;
    }
    return high;
}

/* Compute a(n) from a given t value */
static seq_t a_from_t(seq_t t)
{
    seq_t t2   = t * t;                           /* t^2          */
    seq_t part = t2 + t - (seq_t)1;              /* -1 + t + t^2 */
    seq_t s    = (seq_t)13 * ((seq_t)26 + (seq_t)10 * t + t2);
    seq_t r    = seq_isqrt(s);                    /* sqrt part    */
    return (seq_t)13 * part * r;                  /* final result */
}

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                    /* not used, guards against under‑flow */

    seq_t t;
    if (n == 1)
        t = (seq_t)13;
    else if (n == 2)
        t = (seq_t)23377;
    else if (n == 3)
        t = (seq_t)30349813;
    else {
        /* recurrence for n ≥ 4 */
        seq_t t_prev2 = (seq_t)13;          /* t[1] */
        seq_t t_prev1 = (seq_t)23377;       /* t[2] */
        seq_t t_curr  = (seq_t)30349813;    /* t[3] */
        for (idx_t i = 4; i <= n; ++i) {
            seq_t t_next = (seq_t)1299 * t_curr
                         - (seq_t)1299 * t_prev1
                         + t_prev2;
            t_prev2 = t_prev1;
            t_prev1 = t_curr;
            t_curr = t_next;
        }
        t = t_curr;
    }

    return a_from_t(t);
}
