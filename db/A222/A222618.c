/*
 * A222618  Multiples of 10 that are sum of two consecutive primes.
 * https://oeis.org/A222618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A222618: multiples of 10 that are sum of two consecutive primes.
   The function generates primes on demand and collects sums that are divisible by 10.
   When the requested index n is not yet found, it continues generating primes until
   the nth such sum exists.  Results are cached between function calls. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* static caches for primes and sums */
    static uint64_t *primes = NULL;
    static size_t  prime_cap = 0;
    static size_t  prime_cnt = 0;
    static seq_t  *sums   = NULL;
    static size_t  sum_cap = 0;
    static size_t  sum_cnt = 0;

    /* initialise prime list with 2 and 3 */
    if (prime_cnt == 0) {
        prime_cap = 8;
        primes = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        primes[0] = 2;
        primes[1] = 3;
        prime_cnt = 2;
    }

    /* grow sums array if needed */
    if (sum_cap == 0) {
        sum_cap = 16;
        sums = (seq_t *)malloc(sum_cap * sizeof(seq_t));
    }

    /* keep generating primes until we have at least n sums */
    while (sum_cnt < (size_t)n) {
        /* get next prime after the last one in the list */
        uint64_t next = next_prime(primes[prime_cnt - 1]);

        /* expand primes cache if necessary */
        if (prime_cnt >= prime_cap) {
            size_t new_cap = prime_cap * 2;
            uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
            primes = tmp;
            prime_cap = new_cap;
        }
        primes[prime_cnt++] = next;

        /* compute sum of the new prime and its predecessor */
        seq_t sum = (seq_t)primes[prime_cnt - 2] + (seq_t)primes[prime_cnt - 1];
        if (sum % (seq_t)10 == (seq_t)0) {
            /* store the sum if it is a multiple of 10 */
            if (sum_cnt >= sum_cap) {
                size_t new_cap = sum_cap * 2;
                seq_t *tmp = (seq_t *)realloc(sums, new_cap * sizeof(seq_t));
                sums = tmp;
                sum_cap = new_cap;
            }
            sums[sum_cnt++] = sum;
        }
    }

    return sums[n - 1];
}
