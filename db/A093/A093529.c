/*
 * A093529  Pi*denominators of odd raw moments in the distribution of line
 *          lengths for lines picked at random in the unit disk.
 * https://oeis.org/A093529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of the odd raw moment rational part * (the
 *            integer part of the denominator when the Pi factor * is
 *            removed). Sample values: a(1)=45, a(2)=525,
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for n=0, return 0 */
    int k = (int)n;
    int limit = 2 * k + 4;                /* max factorial argument */

    /* Sieve primes up to limit */
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) return 0;               /* allocation failure */
    for (int i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; (long long)i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= limit; j += i) {
                is_prime[j] = false;
            }
        }
    }

    seq_t result = 1;
    for (int p = 2; p <= limit; ++p) {
        if (!is_prime[p]) continue;

        /* exponent of p in denominator  = exp_fact(2k+2) + exp_fact(2k+4) */
        int e_den = 0;
        int m = 2 * k + 2;
        for (int pp = p; m >= pp; pp *= p) e_den += m / pp;
        m = 2 * k + 4;
        for (int pp = p; m >= pp; pp *= p) e_den += m / pp;

        /* exponent of p in numerator  = exp_fact(2k) + exp_fact(k+1) + exp_fact(k+2) */
        int e_num = 0;
        m = 2 * k;
        for (int pp = p; m >= pp; pp *= p) e_num += m / pp;
        m = k + 1;
        for (int pp = p; m >= pp; pp *= p) e_num += m / pp;
        m = k + 2;
        for (int pp = p; m >= pp; pp *= p) e_num += m / pp;

        /* extra factor 2^(4k+7) coming from 2 * 4^(2k+3) * Pi */
        if (p == 2) {
            e_num += 4 * k + 7;
        }

        int e = e_den - e_num;
        if (e > 0) {
            seq_t pow = big_pow((seq_t)p, (seq_t)e);
            result *= pow;
        }
    }

    free(is_prime);
    return result;
}
