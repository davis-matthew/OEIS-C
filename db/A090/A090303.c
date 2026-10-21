/*
 * A090303  Begin with n and consider numbers obtained by successively
 *          subtracting 2,3,5,7,11,...; a(n) = largest prime that arises,
 *          i.e., largest prime of the form n - sum prime(r), or 0 if no such
 *          prime exists.
 * https://oeis.org/A090303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime of the form n - sum_{i=1}^k prime(i); if no
 *            such prime exists, a(n) = 0
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* For n ≤ 2 the result is 0. */
    if (n <= 2) return 0;

    seq_t best = 0;          /* largest prime found so far */
    seq_t sum  = 0;          /* cumulative sum of primes */
    seq_t N    = (seq_t)n;   /* promote index to seq_t */

    /* limit = N - 2; when sum exceeds this value the candidate
       n - sum will be < 2 and cannot be prime. */
    seq_t limit = N - 2;

    for (idx_t k = 1; ; ++k) {
        seq_t p = (seq_t)get_prime(k);  /* k-th prime */
        sum += p;
        if (sum > limit) break;          /* candidate < 2 */

        seq_t cand = N - sum;            /* n - sum of first k primes */
        if (is_prime_big(cand) && cand > best)
            best = cand;
    }

    return best;
}
