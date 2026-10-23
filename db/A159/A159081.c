/*
 * A159081  Let d be the largest element of A008578 which divides n, then a(n)
 *          is the position of d in A008578.
 * https://oeis.org/A159081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime_pi(gpf(n)) + 1, with a(1) = 1 (A159081)
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   gpf, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gpf.h"
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, return 0 for safety */
    if (n == 1) return (seq_t)1;   /* largest divisor is 1 */
    uint64_t p = gpf((uint64_t)n); /* largest prime factor of n */
    seq_t pi = prime_pi((idx_t)p); /* number of primes ≤ p */
    return pi + (seq_t)1;          /* position in A008578 */
}
