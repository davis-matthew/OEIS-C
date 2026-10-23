/*
 * A123623  Smallest k>1 such that mu(n*k) = mu(n), where mu=A008683.
 * https://oeis.org/A123623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of the two smallest primes not dividing n when n
 *            is squarefree; otherwise a(n) = 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _is_squarefree_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/_is_squarefree_u64.h"

seq_t a(idx_t n)
{
    /* If n is not squarefree, the answer is always 2. */
    if (!_is_squarefree_u64(n))
        return (seq_t)2;

    /* Find the first two primes not dividing n. */
    idx_t first = 0, second = 0;
    for (idx_t cand = 2; ; ++cand) {
        if (!is_prime(cand))
            continue;
        if (n % cand != 0) {
            if (first == 0)
                first = cand;
            else {
                second = cand;
                break;
            }
        }
    }

    /* Return the product of the two primes in seq_t type. */
    return (seq_t)first * (seq_t)second;
}
