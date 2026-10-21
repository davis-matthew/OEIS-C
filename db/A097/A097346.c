/*
 * A097346  Numerators of a Fibonacci related transform of 1/(n+1).
 * https://oeis.org/A097346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, frac
 * Formula:   a(n) = A003418(n) * sum_{k=0..floor(n/2)} binom(n-k, k)/(k+1)
 *            where A003418(n) = lcm(1, 2, …, n+1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binom, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* 1. compute A003418(n) = lcm(1 .. n+1) */
    seq_t lcm_val = 1;
    for (idx_t i = 2; i <= n + 1; ++i) {
        seq_t g = seq_gcd(lcm_val, (seq_t)i);
        lcm_val = (lcm_val / g) * (seq_t)i;   /* lcm = lcm_prev / g * i */
    }

    /* 2. compute the rational sum */
    seq_t sum_num = 0;
    seq_t sum_den = 1;
    for (idx_t k = 0; k <= n / 2; ++k) {
        seq_t term_num = binom(n - k, k);      /* binomial coefficient */
        seq_t term_den = (seq_t)(k + 1);       /* denominator of the term */

        seq_t new_num = sum_num * term_den + term_num * sum_den;
        seq_t new_den = sum_den * term_den;
        seq_t g = seq_gcd(new_num, new_den);
        sum_num = new_num / g;
        sum_den = new_den / g;
    }

    /* 3. multiply by lcm and divide by denominator (exact division) */
    seq_t numerator = lcm_val * sum_num;
    seq_t result = numerator / sum_den;

    return result;
}
