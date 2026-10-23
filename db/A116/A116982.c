/*
 * A116982  Distance between prime neighbors of 4n.
 * https://oeis.org/A116982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime_next(4*n) - prime_prev(4*n)
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   next_prime, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* distance between the prime neighbors of 4n */

#include "../utils/prev_prime.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* no definition for n = 0, return 0 */
    idx_t m   = 4 * n;       /* 4*n */
    idx_t prev = prev_prime(m);
    idx_t next = next_prime(m);
    return (seq_t)(next - prev);
}
