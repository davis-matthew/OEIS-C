/*
 * A168499  Primes n of the form 100p+q with primes p and q, 98>p>q>10.
 * https://oeis.org/A168499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base, easy
 * Formula:   Primes of the form 100*p+q where p and q are primes, 98>p>q>10.
 *            The sequence lists the primes 100*p+q in increasing order
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n) {
    /* Prime candidates are formed from two‑digit primes p and q.
       There are 21 such primes: 11,13,…,97.  For each pair p>q>10
       we test whether 100*p+q is prime.  The discovered primes are
       stored the first time the function is called. */
    static seq_t primes[210];
    static size_t count = 0;
    if (count == 0) {
        const uint64_t two_digit[] = {
            11, 13, 17, 19, 23, 29, 31, 37,
            41, 43, 47, 53, 59, 61, 67, 71,
            73, 79, 83, 89, 97
        };
        const size_t num_primes = sizeof(two_digit)/sizeof(two_digit[0]);

        for (size_t i = 0; i < num_primes; ++i) {
            seq_t p = (seq_t)two_digit[i];
            for (size_t j = 0; j < i; ++j) {
                seq_t q = (seq_t)two_digit[j];
                seq_t num = 100 * p + q;
                if (is_prime_small(num)) {
                    primes[count++] = num;
                }
            }
        }
    }

    if (n == 0 || n > count)
        return (seq_t)0;
    return primes[(size_t)n - 1];
}
