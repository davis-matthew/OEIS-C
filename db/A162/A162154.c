/*
 * A162154  Odd-indexed terms are the number of consecutive prime numbers
 *          until a composite, even-indexed terms are the number of
 *          consecutive composite numbers until a prime.
 * https://oeis.org/A162154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of consecutive primes (odd n) or composites (even
 *            n) * defined by scanning the positive integers starting from 2.
 *            * This implementation recomputes the sequence from scratch each
 *            call. * No static cache is used to remain compatible with both
 *            C11 and C++17 * when seq_t may be an arbit...
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined, defensive */

    bool prime_run = true;   /* true → counting primes, false → composites */
    idx_t run_len = 0;       /* length of the current run */
    idx_t num = 2;           /* next integer to examine */
    idx_t produced = 0;      /* number of terms produced so far */

    seq_t result = 0;

    while (produced < n) {
        bool p = is_prime(num);

        if (prime_run) {
            if (p) {
                run_len++;                     /* extend prime run */
            } else {                           /* first composite encountered */
                result = (seq_t)run_len;       /* this is the next term */
                produced++;
                if (produced == n) return result;
                prime_run = false;              /* switch to composite run */
                run_len = 1;                    /* count the composite itself */
            }
        } else { /* composite run */
            if (!p) {
                run_len++;                     /* extend composite run */
            } else {                           /* first prime encountered */
                result = (seq_t)run_len;       /* this is the next term */
                produced++;
                if (produced == n) return result;
                prime_run = true;               /* switch to prime run */
                run_len = 1;                    /* count the prime itself */
            }
        }

        num++;
    }

    return result;
}
