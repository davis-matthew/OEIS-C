/*
 * A086168  a(n) = sum of the first n upper twin primes.
 * https://oeis.org/A086168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 68, u32 n <= 8730, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = sum of the upper primes of the first n upper twin prime pairs.
 * The function uses incremental prime testing and memoises the cumulative sums.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    static seq_t *cum_sums = NULL;
    static size_t cum_size = 0, cum_cap = 0;
    static uint64_t next_candidate = 5;
    static bool first_call = true;
    static size_t twin_cnt = 0;

    if (n == 0)
        return 0;   /* Undefined for n=0, but return 0 to be safe */

    /* Generate twin upper primes until we have at least n of them */
    while (twin_cnt < n)
    {
        /* Find the next candidate that is both prime and has a prime twin below */
        if (first_call) {
            next_candidate = 5;          /* start after the pair (3,5) */
            first_call = false;
        }
        while (!(is_prime(next_candidate) && is_prime(next_candidate - 2)))
            next_candidate += 2;

        /* Upper twin prime found */
        seq_t upper = (seq_t)next_candidate;
        seq_t new_sum = (cum_size == 0) ? upper : cum_sums[cum_size - 1] + upper;
        append_candidate(&cum_sums, &cum_size, &cum_cap, new_sum);

        twin_cnt++;
        next_candidate += 2;   /* move to the next odd candidate */
    }

    return cum_sums[n - 1];   /* a(1) is the first term */
}
