/*
 * A169281  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^29 = I.
 * https://oeis.org/A169281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A169281 – Number of reduced words of length n in a Coxeter
 *  group with 28 generators S_i and relations (S_i)^2 = (S_i S_j)^29 = I.
 *
 *  The sequence satisfies the linear recurrence obtained from the rational
 *  generating function
 *      G(t) = (t^29 + 2t^28 + … + 2t + 1) / (351t^29 – 26t^28 – … – 26t – 26).
 *
 *  Let cum[i] = Σ_{k=0}^{i-1} a(k).  Then for 0 ≤ i ≤ 28
 *      a(i) = 26 * cum[i] + 2,
 *  and for i ≥ 29
 *      a(i) = 26 * (cum[i] – cum[i-28]) – 351 * a(i-29).
 *
 *  This implements the recurrence with memoisation.  The function is
 *  thread‑unsafe but satisfies all interface requirements.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static storage for previously computed terms */
    static seq_t *a_arr  = NULL;   /* a[0..max_n]          */
    static seq_t *cum_arr = NULL;  /* cum[0..max_n+1]      */
    static idx_t  max_n   = 0;      /* largest index computed */

    /* initialise on first call */
    if (!a_arr) {
        a_arr   = (seq_t *)malloc( sizeof(seq_t) );   /* a[0]   */
        cum_arr = (seq_t *)malloc( 2 * sizeof(seq_t) ); /* cum[0], cum[1] */

        /* base values a(0) = 1, cum(0) = 0, cum(1) = a(0) */
        a_arr[0]   = (seq_t)1;
        cum_arr[0] = (seq_t)0;
        cum_arr[1] = a_arr[0];
        max_n = 0;
    }

    /* grow arrays if necessary */
    if (n > max_n) {
        size_t new_size  = (size_t)(n + 1);
        size_t new_cum   = (size_t)(n + 2);

        a_arr = (seq_t *)realloc(a_arr, new_size * sizeof(seq_t));
        cum_arr = (seq_t *)realloc(cum_arr, new_cum * sizeof(seq_t));

        /* iterate over new indices */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            if (i <= 28) {                     /* a(i) = 26 * cum(i) + 2 */
                a_arr[i] = (seq_t)26 * cum_arr[i] + (seq_t)2;
            } else {                            /* i ≥ 29 */
                seq_t sum_last28 = cum_arr[i] - cum_arr[i - 28];
                a_arr[i] = (seq_t)26 * sum_last28 - (seq_t)351 * a_arr[i - 29];
            }
            cum_arr[i + 1] = cum_arr[i] + a_arr[i];
        }
        max_n = n;
    }

    return a_arr[n];
}
