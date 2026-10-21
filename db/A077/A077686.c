/*
 * A077686  2^(n-1) - (prime(n) mod n).
 * https://oeis.org/A077686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-1) - (prime(n) mod n)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   get_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    uint32_t p = get_prime(n);     /* nth prime */
    seq_t power = pow2(n - 1);     /* 2^(n-1)  */
    seq_t mod   = (seq_t)(p % (uint64_t)n);

    return power - mod;
}
