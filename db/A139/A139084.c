/*
 * A139084  a(n) = (smallest prime-power among the largest powers dividing n
 *          of each prime dividing n) * (smallest prime-power among the
 *          largest powers dividing (n+1) of each prime dividing (n+1)).
 * https://oeis.org/A139084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A034684(n) * A034684(n+1) where A034684(n) is the
 *            smallest prime power among the largest powers dividing n of each
 *            prime dividing n. For n = 1 it is 1
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* ------------------------------------------------------------------
   Return the smallest prime factor of x, or x itself if x is prime.
   Works for 1 <= x <= UINT64_MAX.                                              
   ------------------------------------------------------------------ */
static uint64_t _smallest_prime_factor(uint64_t x)
{
    if (x < 2) return 1;                /* only called for x >= 1   */
    if ((x & 1ULL) == 0) return 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return d;
    return x;                           /* x is prime                */
}

/* ------------------------------------------------------------------
   Compute A034684(n).  For n=1 return 1.  For n>1, factor n, compute
   the largest power of each prime dividing n, then return the minimal
   of those powers.                                               
   ------------------------------------------------------------------ */
static seq_t min_prime_power(uint64_t n)
{
    if (n <= 1) return (seq_t)1;

    seq_t min_pp = 0;
    uint64_t tmp  = n;

    while (tmp > 1) {
        uint64_t p = _smallest_prime_factor(tmp);
        /* compute p^e where e is the exponent of p in n */
        __uint128_t ppow = 1;          /* use 128‑bit to avoid overflow on the way */
        while (tmp % p == 0) {
            tmp   /= p;
            ppow  *= p;
        }
        seq_t pp = (seq_t)ppow;
        if (min_pp == 0 || pp < min_pp) min_pp = pp;
    }
    return min_pp;
}

/* ------------------------------------------------------------------
   Sequence function.  By definition a(1) corresponds to the first
   listed term.  All calls are independent; no memoisation is used.
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    uint64_t n64 = (uint64_t)n;
    seq_t p1 = min_prime_power(n64);
    seq_t p2 = min_prime_power(n64 + 1ULL);
    return p1 * p2;
}
