/*
 * A120268  Numerator of Sum_{k=1..n} 1/(2*k-1)^2.
 * https://oeis.org/A120268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Sum_{k=1..n} 1/(2*k-1)^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* outside the offset */

    /* static cache of numerators and denominators up to the largest n seen   */
    static seq_t *num_arr   = NULL;       /* numerator of the reduced sum   */
    static seq_t *den_arr   = NULL;       /* denominator of the reduced sum   */
    static idx_t  max_n     = 0;           /* largest n already computed      */
    static idx_t  capacity  = 0;           /* current allocation size         */

    /* initialise storage on first call                                 */
    if (capacity == 0) {
        capacity = 16;
        num_arr  = (seq_t *)malloc((capacity + 1) * sizeof(seq_t));
        den_arr  = (seq_t *)malloc((capacity + 1) * sizeof(seq_t));
        if (!num_arr || !den_arr) return (seq_t)0;   /* allocation failed */

        /* sum for n = 1: 1 / 1^2 = 1/1   */
        num_arr[1] = (seq_t)1;
        den_arr[1] = (seq_t)1;
        max_n = 1;
    }

    /* grow the cache if necessary                                        */
    if (n > capacity) {
        idx_t new_cap = capacity == 0 ? 16 : capacity;
        while (n > new_cap) new_cap <<= 1;
        seq_t *tmp1 = (seq_t *)realloc(num_arr, (new_cap + 1) * sizeof(seq_t));
        seq_t *tmp2 = (seq_t *)realloc(den_arr, (new_cap + 1) * sizeof(seq_t));
        if (!tmp1 || !tmp2) return (seq_t)0;    /* allocation failed */
        num_arr  = tmp1;
        den_arr  = tmp2;
        capacity = new_cap;
    }

    /* compute values up to n                                              */
    while (max_n < n) {
        idx_t i = max_n + 1;

        /* term = 1 / ( (2*i-1)^2 )                                       */
        seq_t odd     = (seq_t)(2 * i - 1);    /* odd number */
        seq_t t_den   = odd * odd;             /* denominator of the new term */

        /* add new term to current reduced fraction                       */
        seq_t new_num = num_arr[max_n] * t_den + den_arr[max_n];
        seq_t new_den = den_arr[max_n] * t_den;

        /* reduce the fraction                                            */
        seq_t g = gcd(new_num, new_den);
        new_num /= g;
        new_den /= g;

        num_arr[i] = new_num;
        den_arr[i] = new_den;
        max_n = i;
    }

    return num_arr[n];
}
