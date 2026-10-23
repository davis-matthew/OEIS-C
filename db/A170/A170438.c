/*
 * A170438  Number of reduced words of length n in Coxeter group on 45
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^44 = I.
 * https://oeis.org/A170438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of reduced words of length n in the Coxeter group
 *            with 45 generators S_i, (S_i)^2 = I, (S_i S_j)^44 = I. The
 *            generating function is (t^44 + 2t^43 + ... + 2t + 1) / (946t^44
 *            - 43t^43 - ... - 43t + 1). The sequence satisfies a(0)=1, a(n)=2
 *            + 43·∑_{k=0}^{n-1}a(k) for n<44 and a(44)=1 +...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Handle trivial case */
    if (n == 0u) {
        return (seq_t)1;
    }

    /* We need to keep the last 45 values for the recurrence */
    seq_t buf[45] = {0};
    seq_t sum_all = 0;          /* sum of all a(k) computed so far (k ≤ current) */
    seq_t sum0to42 = 0;         /* sum of a(0)…a(42) after a(42) is added */

    /* a(0) */
    buf[0] = (seq_t)1;
    sum_all += buf[0];

    /* Compute a(1)…a(43) using a(n)=2 + 43·sum_all previous values */
    for (idx_t i = 1; i <= 43; ++i) {
        seq_t val = (seq_t)2 + (seq_t)43 * sum_all;
        buf[i] = val;
        sum_all += val;
        if (i == 42) {
            sum0to42 = sum_all;  /* after adding a(42) */
        }
    }

    /* Compute a(44) = 1 + 43·sum0to42 - 946·a(0) */
    seq_t val44 = (seq_t)1 + (seq_t)43 * sum0to42 - (seq_t)946 * buf[0];
    buf[44] = val44;
    sum_all += val44;          /* includes a(44) */

    if (n <= 44u) {
        return buf[n % 45];
    }

    /* For n ≥ 45, use the recurrence:
       a(n) = 43 * sum_{k=n-43}^{n-1} a(k) - 946 * a(n-44)   */
    /* Initialise the sliding window sum of the last 43 terms:
       For n = 45 it must be a(2) + … + a(44) */
    seq_t sum43 = sum_all - buf[0] - buf[1];   /* sum of a(2)…a(44) */

    for (idx_t i = 45; i <= n; ++i) {
        seq_t old_historic = buf[(i - 44) % 45]; /* a(i-44) needed for recurrence */
        seq_t val = (seq_t)43 * sum43 - (seq_t)946 * old_historic;

        buf[i % 45] = val;

        /* Update sliding window sum for the next iteration:
           remove a(i-43), add a(i) */
        sum43 -= buf[(i - 43) % 45];
        sum43 += val;
    }

    return buf[n % 45];
}
