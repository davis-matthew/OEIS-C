/*
 * A071990  Numerators of the partial sums of the reciprocals of the upper
 *          members of twin prime pairs.
 * https://oeis.org/A071990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = numerator(Sum_{k=1..n} 1 / A006512(k)) where A006512(k)
 *            is the upper member of the k-th twin prime pair
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined but return 0 for safety */

    /* Estimate an upper bound for the nth upper twin prime.
       For n <= 10 use a small bound; otherwise use a heuristic. */
    idx_t limit;
    if (n < 10)
        limit = 100;
    else {
        double nn = (double)n;
        double est = nn * (log(nn) + log(log(nn))) * 2.0 + 100.0;
        limit = (idx_t)est;
    }

    /* A simple failure-safe loop: if the chosen limit is not
       sufficient (e.g., n is large), double it and retry. */
    while (true) {
        uint8_t *is_prime = (uint8_t *)malloc(limit + 1);
        if (!is_prime)
            return 0; /* allocation failure */

        /* Sieve of Eratosthenes */
        for (idx_t i = 0; i <= limit; i++)
            is_prime[i] = 1;
        is_prime[0] = is_prime[1] = 0;
        for (idx_t i = 2; i * i <= limit; i++) {
            if (is_prime[i]) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_prime[j] = 0;
            }
        }

        seq_t num = 0; /* numerator of the running sum */
        seq_t den = 1; /* denominator of the running sum */
        idx_t count = 0;

        for (idx_t p = 3; p + 2 <= limit; p++) {
            if (is_prime[p] && is_prime[p + 2]) {
                count++;
                seq_t q = (seq_t)(p + 2);          /* upper twin prime */
                seq_t new_num = num * q + den;      /* num/q + 1/q */
                seq_t new_den = den * q;
                seq_t g = gcd(new_num, new_den);   /* reduce */
                num = new_num / g;
                den = new_den / g;
                if (count == n) {
                    free(is_prime);
                    return num;
                }
            }
        }

        /* Not enough twin primes found: enlarge limit and retry. */
        free(is_prime);
        limit *= 2;
    }
}
