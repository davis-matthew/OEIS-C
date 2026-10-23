/*
 * A135218  A007318^(-5) * A001286, 5th inverse binomial transform of the Lah
 *          numbers.
 * https://oeis.org/A135218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A135218
   Formula: a(n) = Σ_{k=0}^n C(n,k) * ((k+1)! * (k+1)(k+2)/2) * (-5)^(n–k)  */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* special case n == 0 */
    if (n == 0)
        return (seq_t)1;

    /* sums of positive and negative contributions */
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    /* 5^n */
    seq_t power5 = 1;
    for (idx_t i = 0; i < n; ++i) {
        power5 *= (seq_t)5;
    }

    /* binomial coefficient C(n,0) and factorial factK = k! (initial 0! = 1) */
    seq_t binom = 1;   /* C(n,0) */
    seq_t factK = 1;   /* 0! */

    for (idx_t k = 0; k <= n; ++k) {
        /* compute A001286(k) = (k+1)! * (k+1)(k+2) / 2 */
        seq_t arr = factK * (k + 1);          /* (k+1)!     */
        arr *= (k + 1) * (k + 2);            /* * (k+1)(k+2) */
        arr /= 2;                            /* /2          */

        /* term = arr * C(n,k) * 5^(n-k) */
        seq_t term = arr * binom * power5;

        /* add to positive or negative partial sum */
        if (((n - k) & 1ULL) == 0)
            sum_pos += term;
        else
            sum_neg += term;

        /* prepare for next k (if any) */
        if (k == n)
            break;

        /* update factorial: factK *= (k+1)  -> becomes (k+1)! for next loop */
        factK *= (k + 1);

        /* update binomial coefficient: C(n,k+1) = C(n,k)*(n-k)/(k+1) */
        binom = binom * (n - k) / (k + 1);

        /* update power5: divide by 5 to get 5^(n-(k+1)) */
        power5 /= 5;
    }

    /* a(n) = sum_pos – sum_neg (always non‑negative) */
    return sum_pos - sum_neg;
}
