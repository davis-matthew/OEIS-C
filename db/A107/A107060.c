/*
 * A107060  a(n) = smallest number m >=0 such that n*prime(n)-m is a prime.
 * https://oeis.org/A107060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m ≥ 0 such that n*prime(n) - m is prime
 * Range:     u8 n <= 9, u16 n <= 795, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"   /* get_prime(idx_t) returns the idx‑th prime as seq_t */
#include "../utils/is_prime.h"    /* is_prime(uint64_t) returns true if number is prime */

seq_t a(idx_t n)
{
    /* Compute n * prime(n).  The result fits in seq_t for all n up to the limits
       considered (n ≤ 10 000 ⇒ n*prime(n) < 1.1·10⁹).                     */
    seq_t p = get_prime(n);
    seq_t prod = (seq_t)n * p;

    /* Search downward for the largest prime ≤ prod.  The counter is kept as
       uint64_t because prod is small enough.                                */
    uint64_t prod64 = (uint64_t)prod;
    uint64_t cand = prod64;
    while (cand >= 2 && !is_prime(cand))
        --cand;
    if (cand < 2) cand = 2;            /* safety: 2 is the smallest prime */

    /* The desired m is the difference between prod and the found prime.   */
    seq_t m = prod - (seq_t)cand;
    return m;
}
