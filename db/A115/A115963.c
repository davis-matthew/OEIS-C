/*
 * A115963  Numerator of Sum_{i=1..n} 1/prime(i)^3.
 * https://oeis.org/A115963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = numerator of Sum_{i=1..n} 1 / prime(i)^3 (offset 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   _first_n_primes, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/_first_n_primes.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    uint32_t *primes = NULL;
    size_t primes_cnt = 0;

    _first_n_primes(n, &primes, &primes_cnt);
    if (primes_cnt != (size_t)n) {
        free(primes);
        return 0;
    }

    seq_t num = 0;  /* numerator of the partial sum */
    seq_t den = 1;  /* denominator of the partial sum */

    for (size_t i = 0; i < n; ++i) {
        uint32_t p = primes[i];
        seq_t pp = (seq_t)p * p;   /* p² */
        seq_t p3 = pp * p;         /* p³ */

        seq_t new_num = num * p3 + den;   /* num/den + 1/p³ = (num*p3 + den) / (den*p3) */
        seq_t new_den = den * p3;

        seq_t g = gcd(new_num, new_den);
        if (g != 0) {          /* reduce fraction exactly */
            num = new_num / g;
            den = new_den / g;
        } else {
            num = new_num;
            den = new_den;
        }
    }

    free(primes);
    return num;
}
