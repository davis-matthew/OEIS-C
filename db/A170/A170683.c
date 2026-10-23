/*
 * A170683  Number of reduced words of length n in Coxeter group on 50
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* a(n) = Number of reduced words of length n in the Coxeter group
 * on 50 generators with relations (S_i)^2 = (S_i S_j)^49 = I.
 *
 * Initial values:
 *   a(0) = 1
 *   a(i) = 2 + 48 * Σ_{k=0}^{i-1} a(k)          for 1 ≤ i ≤ 48
 *   a(49) = 1 + 48 * Σ_{k=1}^{48} a(k) - 1176*a(0)
 *
 * For i ≥ 50 the sequence satisfies the linear recurrence
 *   a(i) = 48 * Σ_{k=1}^{48} a(i-k) - 1176 * a(i-49)
 *
 * The function below computes a(n) in O(n) time using a circular
 * buffer of length 49 and running sums.  No external memory growth
 * or expensive operations are required.  It works for any seq_t/
 * idx_t combination that satisfies the constraints of the build. */

seq_t a(idx_t n)
{
    if (n == 0ULL)
        return (seq_t)1;

    /* Circular buffer of the last 49 values. */
    seq_t buf[49] = {0};
    buf[0] = (seq_t)1;                /* a(0) */
    seq_t total_sum = buf[0];          /* sum of all computed values so far   */

    /* Compute a(1)..a(48) if requested. */
    for (idx_t i = 1; i <= 48 && i <= n; ++i)
    {
        seq_t val = (seq_t)2 + (seq_t)48 * total_sum; /* a(i) = 2 + 48*sum_{<i} */
        buf[i] = val;
        total_sum += val;
    }

    if (n <= 48)                /* n was reached in the loop above */
        return buf[n];

    /* Now we've computed a(0) .. a(48).  Prepare to compute a(49). */
    seq_t sum48 = total_sum - buf[0];              /* Σ_{k=1}^{48} a(k) */

    /* a(49) computation */
    seq_t val49 = (seq_t)1 + (seq_t)48 * sum48 - (seq_t)1176 * buf[0];
    buf[0] = val49;               /* overwrite a(0); no longer needed */
    if (n == 49ULL)
        return val49;

    /* Update sum48 to contain Σ_{k=2}^{49} a(k) for the next step. */
    sum48 += val49;
    sum48 -= buf[1];               /* subtract a(1) */

    /* Compute a(50)..a(n) using the recurrence. */
    for (idx_t i = 50; i <= n; ++i)
    {
        idx_t idx = i % 49;        /* position in the circular buffer */
        seq_t a_n_minus_49 = buf[idx];         /* a(i-49) */
        seq_t val = (seq_t)48 * sum48 - (seq_t)1176 * a_n_minus_49;

        buf[idx] = val;            /* store a(i) */

        if (i == n)                /* reached target */
            return val;

        /* Adjust sum48 for the next iteration:
         * new sum48 = old sum48 + a(i) - a(i-48)  */
        sum48 += val;
        sum48 -= buf[(i - 48) % 49];
    }

    /* Should never be reached, but keep the compiler happy. */
    return buf[n % 49];
}
