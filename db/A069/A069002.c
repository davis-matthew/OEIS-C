/*
 * A069002  Smallest integer d such that n^2 + (n-d)^2 is a prime number.
 * https://oeis.org/A069002
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest d ≥ 1 such that n^2 + (n − d)^2 is prime, for n
 *            ≥ 2. If no such d ≤ n − 1 exists (which never happens for the
 *            tested range) the function returns n − 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;               /* undefined for n < 2 */

    /* Use a wider intermediate type to avoid overflow on n^2 + (n-d)^2.
       All values used in is_prime are cast to idx_t afterwards. */
    unsigned __int128 n_sq = (unsigned __int128)n * (unsigned __int128)n;

    for (idx_t d = 1; d < n; ++d)
    {
        idx_t m = n - d;
        unsigned __int128 sum = n_sq + (unsigned __int128)m * (unsigned __int128)m;

        /* is_prime takes an idx_t (uint64_t).  The test is only performed
           for sums that fit into 64 bits; for the expected input range
           this is always true. */
        if (is_prime((idx_t)sum))
            return (seq_t)d;
    }

    /* Back‑up return value – should never be reached with valid inputs. */
    return (seq_t)(n - 1);
}
