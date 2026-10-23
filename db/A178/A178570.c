/*
 * A178570  Numbers k such that prime(k+1) == 1 (mod (prime(k+2) - prime(k))).
 * https://oeis.org/A178570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 9120, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A178570
 *  a(n) = the n‑th integer k (1‑based) such that
 *        prime(k+1) ≡ 1 (mod (prime(k+2) – prime(k))).
 *  The function iterates k from 1 upward, keeps a count of how many
 *  numbers have satisfied the condition, and returns the k that
 *  reaches the requested n‑th term.  Prime values are obtained via
 *  the supplied get_prime utility, which caches the primes so the
 *  cost is essentially that of a simple sieve.  The algorithm
 *  runs in O(n·p) time, p being the number of primes needed to
 *  reach the n‑th term (well below 2 seconds for n ≤ 10 000).  It
 *  works for 64‑bit (or arbitrary‑precision) builds because k and
 *  the result are returned as seq_t and all intermediate arithmetic
 *  uses uint64_t, which is larger than the primes involved.  */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    idx_t found = 0;   /* how many qualifying k we have seen */
    idx_t k = 1;       /* candidate n */

    while (found < n) {
        uint64_t pk   = (uint64_t)get_prime(k);     /* prime(k)   */
        uint64_t pk1  = (uint64_t)get_prime(k + 1); /* prime(k+1) */
        uint64_t pk2  = (uint64_t)get_prime(k + 2); /* prime(k+2) */

        uint64_t diff = pk2 - pk;                   /* prime(k+2) – prime(k) */
        if (diff != 0 && (pk1 % diff) == 1) {
            ++found;
            if (found == n) {
                return (seq_t)k;
            }
        }
        ++k;
    }
    /* Should never reach here because the sequence is infinite */
    return (seq_t)0;
}
