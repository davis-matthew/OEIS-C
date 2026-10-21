/*
 * A088198  Distance LQnR(p_n) (A088196) from p_n.
 * https://oeis.org/A088198
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n) - LQnR(prime(n))
 * Range:     u8 n <= 54, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0; /* undefined below OEIS offset */

    /* nth prime */
    seq_t p = (seq_t)get_prime(n);

    /* For primes congruent to 3 (mod 4), −1 is a non‑residue,
       so the largest quadratic non‑residue is p−1 and a(n) = 1. */
    if ((uint64_t)p % 4ULL == 3ULL)
        return (seq_t)1;

    /* For primes ≡ 1 (mod 4) we search downward for the first
       quadratic non‑residue using Legendre symbol.  The search
       terminates after a few iterations for the required range of n. */
    seq_t LQnR = p - 1; /* placeholder; will be overwritten */
    uint64_t exp = ((uint64_t)p - 1ULL) >> 1; /* (p‑1)/2 */

    for (seq_t cand = p - 1; cand > 0; --cand)
    {
        seq_t val = mod_pow(cand, exp, p);
        if (val == p - 1)   /* Legendre symbol = −1 */
        {
            LQnR = cand;
            break;
        }
    }

    return p - LQnR;
}
