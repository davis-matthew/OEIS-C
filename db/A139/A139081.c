/*
 * A139081  a(n) = (largest prime power dividing n) + (largest prime power
 *          dividing (n+1)).
 * https://oeis.org/A139081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A034699(n) + A034699(n+1) where A034699(n) denotes the
 *            largest prime power dividing n (for n ≥ 1)
 * Range:     u8 n <= 177, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 85 terms through n = 964 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the largest prime power dividing a 64‑bit integer n.
   For n ≤ 1 the result is 1. */
static seq_t largest_prime_power_uint64(uint64_t n)
{
    if (n <= 1u)
        return (seq_t)1;

    seq_t max_pw = 1;
    uint64_t rem = n;

    for (uint64_t p = 2; p <= rem / p; ++p)
    {
        if (rem % p == 0)
        {
            seq_t pw = 1;          /* current prime power */
            while (rem % p == 0)
            {
                pw = pw * (seq_t)p;
                rem /= p;
            }
            if (pw > max_pw)
                max_pw = pw;
        }
    }

    if (rem > 1)                   /* remaining prime factor */
    {
        seq_t pw = (seq_t)rem;     /* rem is prime itself */
        if (pw > max_pw)
            max_pw = pw;
    }

    return max_pw;
}

seq_t a(idx_t n)
{
    uint64_t u = (uint64_t)n;          /* idx_t is uint64_t */
    seq_t first  = largest_prime_power_uint64(u);
    seq_t second = largest_prime_power_uint64(u + 1);
    return first + second;
}
