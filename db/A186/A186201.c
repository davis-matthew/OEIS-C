/*
 * A186201  Consider all ways of writing 2n = p + q where p, q are primes, p
 *          <= n and q >= n; let s1(n) = sum of the p's and s2(n) = sum of the
 *          q's; the sequence lists the integers 2n for which s1(n) divides
 *          s2(n).
 * https://oeis.org/A186201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) is the 2n such that the sum of the smaller primes in * all
 *            Goldbach partitions of 2n divides the sum of the larger primes
 * Range:     u8 n <= 10, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   extend_primes_to, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "../utils/is_prime.h"          /* bool is_prime(idx_t x) */
#include "../utils/extend_primes_to.h"

seq_t a(idx_t n)
{
    /* Shared state for prime generation between calls */
    static idx_t *primes = NULL;
    static size_t prime_cnt = 0, prime_cap = 0;
    static idx_t primes_upto = 1;

    idx_t term_found = 0;
    idx_t m = 1;                     /* m = n in definition; starts from 1 */

    while (term_found < n) {
        ++m;
        extend_primes_to(m, &primes, &prime_cnt, &prime_cap, &primes_upto);

        idx_t limit = 2 * m;
        uint64_t sum_p = 0, sum_q = 0;

        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t p = primes[i];
            if (p > m) break;
            idx_t q = limit - p;
            if (q >= m && is_prime(q)) {
                sum_p += (uint64_t)p;
                sum_q += (uint64_t)q;
            }
        }

        if (sum_p != 0 && sum_q % sum_p == 0) {
            ++term_found;
            if (term_found == n) {
                return (seq_t)limit;   /* cast to arbitrary‑precision type */
            }
        }
    }

    /* Should never reach here */
    return (seq_t)0;
}
