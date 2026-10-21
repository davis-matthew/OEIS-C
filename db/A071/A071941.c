/*
 * A071941  Nonprimes for which floor(2^n/n) is even.
 * https://oeis.org/A071941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   powmod_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A071941: Nonprimes for which floor(2^n/n) is even
   Computation is performed by brute force: we test the consecutively
   increasing integers n, keep the non‑prime ones for which
   floor(2^n/n) is even, and return the nth such number. */

#include <stdint.h>
#include "../utils/powmod_uint128.h"

/* Simple trial division primality test, fast enough for
   the values needed (m < ~1e6 in practice). */
static inline int is_prime_uint64(uint64_t n)
{
    if (n < 2) return 0;
    if (n % 2ULL == 0) return n == 2ULL;
    for (uint64_t i = 3ULL; i * i <= n; i += 2ULL)
        if (n % i == 0)
            return 0;
    return 1;
}

/* External function requested by the problem statement. */
seq_t a(idx_t n)
{
    idx_t found = 0;          /* number of terms found so far */
    uint64_t m = 1;           /* current candidate */

    while (found < n)
    {
        if (!is_prime_uint64(m))
        {
            unsigned __int128 mod = (unsigned __int128)m * 2ULL;
            unsigned __int128 rem = powmod_uint128(2ULL, m, mod);
            if (rem < (unsigned __int128)m)   /* even quotient */
            {
                ++found;
                if (found == n)
                    return (seq_t)m;
            }
        }
        ++m;
    }
    /* This point is never reached because the loop always
       returns within the while loop. */
    return (seq_t)0;
}
