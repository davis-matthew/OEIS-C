/*
 * A111179  a(n) = Sum_{k=1..n} prime(k)!, where prime(k) is k-th prime.
 * https://oeis.org/A111179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} prime(k)!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   get_prime, seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/seq_malloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n) {
    /* Static state for memoisation */
    static seq_t *sums = NULL;
    static idx_t sum_cap = 0;          /* allocated capacity of sums array   */
    static seq_t fac = 1;              /* current factorial (product of 1..prev_prime) */
    static seq_t cumulative = 0;       /* sum of factorials up to processed_primes  */
    static uint64_t prev_prime = 0;    /* previous prime value                 */
    static idx_t processed = 0;        /* number of primes already processed    */

    if (n == 0) { /* offset is 1; behaviour for indices <1 is irrelevant */
        return 0;
    }

    /* Expand sums array if necessary */
    if (sum_cap < n) {
        seq_t *tmp = seq_realloc(sums, n);
        sums = tmp;
        sum_cap = n;
    }

    /* If we already have the value cached, return it */
    if (n <= processed) {
        return sums[n - 1];
    }

    /* Compute additional terms up to n */
    for (idx_t k = processed + 1; k <= n; ++k) {
        /* Obtain the k-th prime (as seq_t) and cast to uint64_t */
        seq_t p_seq = get_prime(k);
        uint64_t p = (uint64_t)p_seq;

        /* Update factorial to p! by multiplying the missing numbers */
        for (uint64_t x = prev_prime + 1; x <= p; ++x) {
            fac *= (seq_t)x;
        }

        /* Add new factorial to cumulative sum */
        cumulative += fac;

        /* Store result for index k */
        sums[k - 1] = cumulative;

        /* Prepare for next iteration */
        prev_prime = p;
    }

    processed = n;
    return sums[n - 1];
}
