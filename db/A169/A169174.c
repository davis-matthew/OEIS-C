/*
 * A169174  Number of reduced words of length n in Coxeter group on 17
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^27 = I.
 * https://oeis.org/A169174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1)+15*a(n-2)+…+15*a(n-26)-120*a(n-27) for n≥27,
 *            a(0)=1, a(n)=17*16^(n-1) for 1≤n≤26 (OEIS A169174)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case first. */
    if (n == 0)
        return (seq_t)1;

    /* Build the initial window of the first 27 terms:
       window[0] = a(0), window[1] = a(1), … , window[26] = a(26)   */
    seq_t window[27];
    window[0] = (seq_t)1;
    seq_t pow16 = (seq_t)1;
    for (idx_t i = 1; i <= 26; ++i) {
        window[i] = (seq_t)17 * pow16;   /* a(i) = 17*16^(i-1) */
        pow16 *= (seq_t)16;
    }

    /* For n ≤ 26 we can return the pre‑computed value. */
    if (n <= 26)
        return window[n];

    /* Continue the recurrence for k = 27 … n.
       window[0] holds a(k-27), window[1] holds a(k-26), … ,
       window[26] holds a(k-1) before the step.          */
    for (idx_t k = 27; k <= n; ++k) {
        seq_t sum = (seq_t)0;

        /* 15 * a(k-1) + … + 15 * a(k-26) */
        for (idx_t i = 1; i <= 26; ++i)
            sum += (seq_t)15 * window[i];

        /* subtract 120 * a(k-27) */
        seq_t new_val = sum - (seq_t)120 * window[0];

        /* Shift entries left to make room for the new value. */
        for (idx_t i = 0; i < 26; ++i)
            window[i] = window[i + 1];
        window[26] = new_val;
    }

    /* After the loop, window[26] holds a(n). */
    return window[26];
}
