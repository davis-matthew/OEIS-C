/*
 * A179965  Number of permutations p of [2*n+7] such that |p(i)-p(i-1)| > n
 *          for i>1.
 * https://oeis.org/A179965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((56571*n+1394594)/432)*12^n - (103701/1250)*10^n -
 *            (5/54)*6^n
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 14, u128 n <= 32,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The first four terms (n = 0 … 3) are given directly; for n ≥ 4 the formula
 * holds and is evaluated using integer arithmetic only. */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* initial values for n = 0 … 3 */
    static const seq_t init[4] = {
        (seq_t)5040ULL,
        (seq_t)47622ULL,
        (seq_t)521606ULL,
        (seq_t)6231544ULL
    };

    if (n < 4)
        return init[n];

    /* powers needed for the formula */
    seq_t pow12 = pow_seq((seq_t)12, n);
    seq_t pow10 = pow_seq((seq_t)10, n);
    seq_t pow6  = pow_seq((seq_t)6 , n);

    /* divide the power terms by the appropriate constants; the divisions
     * are exact for all n ≥ 4. */
    seq_t div12 = pow12 / (seq_t)432;   /* 12^n / 432  */
    seq_t div10 = pow10 / (seq_t)1250;  /* 10^n / 1250 */
    seq_t div6  = pow6  / (seq_t)54;    /*  6^n / 54  */

    /* multiplicative coefficients */
    seq_t coeff = (seq_t)56571 * (seq_t)n + (seq_t)1394594;   /* 56571*n + 1394594 */
    seq_t term1 = coeff * div12;                            /* first term  */
    seq_t term2 = (seq_t)103701 * div10;                    /* second term */
    seq_t term3 = (seq_t)5 * div6;                          /* third  term */

    return term1 - term2 - term3;                           /* a(n) */
}
