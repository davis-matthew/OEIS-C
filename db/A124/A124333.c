/*
 * A124333  a(n) = ((n mod phi(n)) +1)th positive integer which is coprime to
 *          n, where phi(n) is number of positive integers which are <= n and
 *          are coprime to n.
 * https://oeis.org/A124333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n mod φ(n))+1)-th positive integer coprime to n
 * Range:     u8 n <= 278, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a simple totient calculation and trial‑division */
#include "../utils/gcd.h"

/* Compute Euler's totient φ(n) for 1 ≤ n ≤ 2^64 (or larger if seq_t can hold it) */
static seq_t phi(idx_t n)
{
    seq_t result = (seq_t)n;           /* start with n */
    idx_t m = n;                       /* working copy for factorisation */

    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            while (m % p == 0)
                m /= p;
            result -= result / (seq_t)p;
        }
    }
    if (m > 1)
        result -= result / (seq_t)m;   /* final prime factor */

    return result;
}

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;               /* cast index to seq_t for calculations */
    seq_t phin = phi(n);               /* φ(n) */
    /* (n mod φ(n)) + 1 gives the ordinal of the desired coprime */
    idx_t rank = (idx_t)(((seq_t)n % phin) + 1);

    idx_t count = 0;                   /* how many coprimes seen so far */
    for (idx_t cand = 1; ; ++cand)
    {
        if (gcd((seq_t)cand, sn) == (seq_t)1)
        {
            ++count;
            if (count == rank)
                return (seq_t)cand;    /* found the required coprime */
        }
    }
}
