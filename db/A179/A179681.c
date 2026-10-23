/*
 * A179681  a(n)=(nextprime(n)+(n+1))(nextprime(n+1)+n)/2.
 * https://oeis.org/A179681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (nextprime(n)+(n+1))*(nextprime(n+1)+n)/2
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                     /* offset 1; behaviour for n<1 is irrelevant */

    uint64_t np1 = next_prime((uint64_t)n);   /* next prime > n   */
    uint64_t np2 = next_prime((uint64_t)(n + 1)); /* next prime > n+1 */

    seq_t sn   = (seq_t)n;                    /* cast index to seq_t */
    seq_t p1   = (seq_t)np1;
    seq_t p2   = (seq_t)np2;

    /* compute (p1 + n + 1) * (p2 + n) / 2  using seq_t arithmetic */
    seq_t part1 = p1 + sn + 1;
    seq_t part2 = p2 + sn;
    seq_t result = part1 * part2 / 2;

    return result;
}
