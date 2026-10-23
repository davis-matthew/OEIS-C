/*
 * A145257  a(n) is the smallest integer > n that is non-coprime to n and has
 *          the same number of 0's in its binary representation as n has.
 * https://oeis.org/A145257
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the smallest integer > n that is non‑coprime to n and has
 *            the same number of 0’s in its binary representation as n has
 * Range:     u8 n <= 28, u16 n <= 190, u32 n <= 190, u64 n <= 2038,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t, gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/bitlen_seq_t.h"
#include "../utils/is_prime.h"

/* popcount for an arbitrary precision integer */
static inline int popcount_seq_t(seq_t x)
{
    int c = 0;
    while (x)
    {
        x &= x - 1;
        ++c;
    }
    return c;
}

/* number of zero bits in the binary representation of x */
static inline int zeros_seq_t(seq_t x)
{
    return (int)bitlen_seq_t(x) - popcount_seq_t(x);
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* offset guard – not used for n>=2 */

    seq_t n_val      = (seq_t)n;
    int   zeros_n    = zeros_seq_t(n_val);

    /* For prime n the gcd condition is satisfied only at multiples of n.
       Searching in multiples eliminates the gcd test for every candidate. */
    if (is_prime(n))
    {
        for (idx_t k = 2;; ++k)
        {
            seq_t m = n_val * (seq_t)k;
            if (zeros_seq_t(m) == zeros_n)
                return m;
        }
    }

    /* General case – brute force search from n+1 upwards. */
    seq_t m = n_val + 1;
    while (true)
    {
        if (gcd(n_val, m) != (seq_t)1 && zeros_seq_t(m) == zeros_n)
            return m;
        ++m;
    }
}
