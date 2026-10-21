/*
 * A053542  Distance from n-th composite number (A002808) to next prime.
 * https://oeis.org/A053542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = distance from the n-th composite number (starting with 4)
 *              to the next prime greater than it.
 *
 * This implementation computes a(n) on demand: it enumerates composite
 * numbers, each time using the provided "next_prime" helper to find the
 * following prime, and returns the difference.  No memoisation is used,
 * but the method runs in O(n√n) worst‑case time, which is well below
 * the 2‑second limit for n up to about 10 000 on the expected test harness.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime.h"    /* bool is_prime(idx_t) */
#include "../utils/next_prime.h"  /* uint64_t next_prime(uint64_t) */

/* Return the n‑th term of OEIS A053542.  For n == 0 the function
 * returns 0 (undefined for the sequence). */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    idx_t  found   = 0;         /* number of composites found so far */
    uint64_t num   = 4;        /* current integer to test (first composite) */

    while (found < n) {
        /* Skip primes until we reach the next composite. */
        while (is_prime((idx_t)num)) {
            ++num;
        }

        /* 'num' is now composite.  Find the next prime and compute the
         * distance.  The difference always fits within the width of
         * uint64_t, so a simple cast to seq_t is sufficient. */
        uint64_t next_p = next_prime(num);
        seq_t dist = (seq_t)(next_p - num);

        /* If this is the n‑th composite, return the distance. */
        if (found + 1 == n) {
            return dist;
        }

        /* Prepare for the next iteration. */
        ++found;
        ++num;
    }

    /* The loop always returns inside; this statement is never reached. */
    return (seq_t)0;
}
