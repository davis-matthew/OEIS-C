/*
 * A170501  Number of reduced words of length n in Coxeter group on 12
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^46 = I.
 * https://oeis.org/A170501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   The sequence satisfies the linear recurrence a(0) = 1 a(n) = 10
 *            * (a(n-1)+…+a(n-45)) + 2 for 1 ≤ n ≤ 45 a(46) = 10 *
 *            (a(1)+…+a(45)) – 55 * a(0) + 1 a(n) = 10 * (a(n-1)+…+a(n-45)) –
 *            55 * a(n-46) for n ≥ 47 This follows from the generating
 *            function N(t)/D(t) with N(t) = t^46 + 2*t^45 + … + 2*t + 1...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* Allocate array of size n+1 */
    size_t sz = (size_t)n + 1;
    seq_t *arr = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!arr)
        return 0;               /* allocation failure – should not happen */

    arr[0] = 1;
    seq_t cumulative = 1;       /* sum of all computed terms so far   */
    seq_t win = 0;              /* sum of the last 45 terms (window) */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t term;
        if (i < 46) {
            /* 1 ≤ i ≤ 45 */
            term = 10 * cumulative + 2;
        } else if (i == 46) {
            /* i = 46 */
            term = 10 * win - 55 * arr[i - 46] + 1;   /* +1 from p(46) */
        } else { /* i ≥ 47 */
            term = 10 * win - 55 * arr[i - 46];
        }

        arr[i] = term;
        cumulative += term;

        /* Update the sliding window of last 45 terms */
        win += term;
        if (i >= 46)   /* window size becomes 45 after adding term i */
            win -= arr[i - 45];
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
