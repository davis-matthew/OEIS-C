/*
 * A082534  Greatest prime p such that p can be expressed as the sum of
 *          consecutive primes with largest prime in the sum = n-th prime.
 * https://oeis.org/A082534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 157, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: for a given n, form the sums of consecutive primes that end
 * with the n‑th prime and return the largest prime among these sums.
 * Implementation uses a dynamically built list of primes and the utility
 * function `_is_prime` from the supplied headers. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/_is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for n = 0, return 0 safely */

    /* --- Dynamic prime list ------------------------------------------------ */
    static uint64_t *primes   = NULL; /* 0‑based list of primes            */
    static size_t   prime_cnt = 0;    /* number of primes stored           */
    static size_t   prime_cap = 0;    /* allocated slots                   */

    /* Need the first n primes.  Grow the list if required. */
    if (prime_cnt < (size_t)n)
    {
        /* Estimate upper bound for the n‑th prime (Rosser’s theorem). */
        size_t limit;
        if ((size_t)n < 6) {
            limit = 15;                /* covers primes 2..13              */
        } else {
            double dn = (double)n;
            limit = (size_t)(dn * (log(dn) + log(log(dn)))) + 3;
        }

        /* Ensure storage for at least n primes. */
        size_t need_cap = (prime_cap > (size_t)n) ? prime_cap : (size_t)n;
        if (prime_cap < need_cap) {
            uint64_t *tmp = (uint64_t *)realloc(primes, need_cap * sizeof(uint64_t));
            if (!tmp) abort();
            primes = tmp;
            prime_cap = need_cap;
        }

        /* Simple sieve up to `limit`.  Rebuild the list entirely. */
        char *composite = (char *)calloc(limit + 1, 1);
        if (!composite) abort();

        size_t cnt = 0;
        for (size_t i = 2; i <= limit; ++i) {
            if (!composite[i]) {
                if (cnt < need_cap) primes[cnt++] = i;
                if (i * i <= limit) {
                    for (size_t j = i * i; j <= limit; j += i)
                        composite[j] = 1;
                }
            }
        }
        free(composite);

        prime_cnt = cnt;   /* now at least n primes stored */
    }
    /* ----------------------------------------------------------------------- */

    /* Compute the largest prime sum of consecutive primes ending at n‑th. */
    uint64_t sum  = 0;
    uint64_t best = 0;

    for (idx_t i = n; i > 0; --i) {
        sum += primes[i - 1];
        if (_is_prime(sum) && sum > best) best = sum;
    }

    return (seq_t)best;
}
