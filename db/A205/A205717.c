/*
 * A205717  (prime(k)-prime(j))/9, where the pairs (k,j) are given by A205712
 *          and A205713.
 * https://oeis.org/A205717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(k) - prime(j)) / 9 where the ordered pairs (k, j)
 *            are all prime index pairs with k > j, enumerated by ascending k
 *            and for each k ascending j. The pair (k, j) is taken from OEIS
 *            sequences A205713 (k) and A205712 (j). The result is the
 *            positive integer quotient
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"   /* provides build_primes_upto */
/* seq_t and idx_t types are supplied by the build environment. */

seq_t a(idx_t n)
{
    /* start with a modest upper bound for the prime sieve */
    idx_t limit = 1000;

    while (1) {
        uint64_t *primes = NULL;      /* array of primes <= limit */
        size_t    prime_cnt = 0;      /* number of primes in the array */

        /* Build prime list up to the current limit. */
        build_primes_upto(limit, &primes, &prime_cnt);
        if (!primes)  /* should never happen; safeguard */
            return 0;

        size_t count = 0;  /* number of qualifying prime pairs seen so far */

        /* Enumerate pairs (k, j) with 1‑based indices: k > j. */
        for (size_t k = 1; k < prime_cnt; ++k) {
            uint64_t pk = primes[k];          /* prime(k) */
            for (size_t j = 0; j < k; ++j) {
                if ((pk - primes[j]) % 9ULL == 0) {
                    ++count;
                    if (count == n) {
                        seq_t result = (seq_t)((pk - primes[j]) / 9ULL);
                        free(primes);
                        return result;
                    }
                }
            }
        }
        /* If we reached here, n was not in the current range.
           Increase the sieve limit and try again.  Free old data. */
        free(primes);
        limit *= 2;   /* double the numeric bound */
    }
}
