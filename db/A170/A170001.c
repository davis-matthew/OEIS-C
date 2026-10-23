/*
 * A170001  Number of reduced words of length n in Coxeter group on 40
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^35 = I.
 * https://oeis.org/A170001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) + 38 * (a(n-1) + a(n-2) + ... + a(n-34)) - 741 *
 *            a(n-35) where p(n) = 1 for n = 0 or n = 35, else 2 for 1 <= n <=
 *            34, and p(n) = 0 for all n > 35. The recurrence follows from the
 *            generating function given in the OEIS entry
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Fastest way: a sliding window of the last 35 values.  No large
       allocation is required and every step takes at most 34
       arithmetic operations. */

    /* Special case: a(0) = 1.  The algorithm below also handles it
       but this explicit return is clearer. */
    if (n == 0)
        return (seq_t)1;

    /* Window of size 35.  Use a static array because 35 is compile‑time
       constant. */
    seq_t buf[35] = {0};
    buf[0] = (seq_t)1;                /* a(0) */
    idx_t i;

    /* Compute subsequent terms up to a(n).  Each new term depends on
       the previous 35 terms. */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t sum = 0;

        /* Constant term p(k) */
        if (k == 35)
            sum += (seq_t)1;
        else if (k <= 34)
            sum += (seq_t)2;
        /* p(k) = 0 for k > 35: nothing to add */

        /* Add 38 * a(k-i) for i = 1 .. min(k,34) */
        idx_t lim = (k < 34) ? k : 34;
        for (i = 1; i <= lim; ++i) {
            idx_t idx = (k - i) % 35;
            sum += (seq_t)38 * buf[idx];
        }

        /* Subtract 741 * a(k-35) when k >= 35 */
        if (k >= 35) {
            idx_t idx = (k - 35) % 35;
            sum -= (seq_t)741 * buf[idx];
        }

        /* Store the newly computed term in the sliding window */
        buf[k % 35] = sum;

        /* If we have reached the requested index, return it */
        if (k == n)
            return sum;
    }

    /* Should never reach here */
    return (seq_t)0;
}
