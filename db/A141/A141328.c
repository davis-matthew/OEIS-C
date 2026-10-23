/*
 * A141328  Infinite array read by antidiagonals: a(m,n) = the smallest
 *          positive integer that is > max(m,n) and is non-coprime to both m
 *          and n.
 * https://oeis.org/A141328
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the smallest positive integer > max(m,n) * where (m,n)
 *            are the coordinates read by antidiagonals of the * infinite
 *            array a(m,n) = smallest positive integer > max(m,n) *
 *            non‑coprime to both m and n. * This implementation factorises m
 *            and n and finds the * minimal multiple that shares a prim...
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Undefined for indices below the offset (1). */
    if (n < 2) return (seq_t)0;

    /* k = 0‑based position in the sequence starting at a(2). */
    idx_t k = n - 2;                 /* k ≥ 0 */

    /* Find diagonal d such that D(d) > k, where
     * D(d) = (d+1)(d+2)/2 is the number of elements
     * up to and including diagonal d. */
    idx_t d = 0;
    while (((d + 1) * (d + 2)) / 2 <= k)
        ++d;

    /* Number of elements before diagonal d. */
    idx_t prev = (d * (d + 1)) / 2;

    /* Position within the diagonal (0‑based). */
    idx_t pos = k - prev;            /* 0 ≤ pos ≤ d */

    /* Coordinates (m, ncol) of the array entry. */
    idx_t m     = 2 + pos;
    idx_t ncol  = 2 + (d - pos);

    /* The largest of the two indices. */
    idx_t M = (m > ncol) ? m : ncol;

    /* Factorise m and ncol.  Only prime divisors are needed. */
    /* Max 32 distinct prime factors is more than enough for m,n≤10000. */
    static const size_t MAX_FACT = 32;
    uint64_t primes1[MAX_FACT], ex1[MAX_FACT];
    uint64_t primes2[MAX_FACT], ex2[MAX_FACT];
    size_t cnt1, cnt2;

    factor_u64((uint64_t)m,   primes1, ex1, &cnt1);
    factor_u64((uint64_t)ncol, primes2, ex2, &cnt2);

    /* Search for the minimal candidate. */
    unsigned __int128 best = ((unsigned __int128) -1);  /* maximum value */

    for (size_t i = 0; i < cnt1; ++i) {
        uint64_t p = primes1[i];
        for (size_t j = 0; j < cnt2; ++j) {
            uint64_t q = primes2[j];
            if (p == q) {
                /* Common prime factor.  Candidate is next multiple of p. */
                unsigned __int128 cand = ((unsigned __int128)M / p + 1) * p;
                if (cand < best) best = cand;
            } else {
                /* Distinct prime factors.  Candidate is next multiple of lcm(p,q)=p*q. */
                unsigned __int128 l = (unsigned __int128)p * q;
                unsigned __int128 cand = ((unsigned __int128)M / l + 1) * l;
                if (cand < best) best = cand;
            }
        }
    }

    /* Convert result to seq_t and return. */
    return (seq_t)best;
}
