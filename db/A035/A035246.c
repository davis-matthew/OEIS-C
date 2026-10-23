/*
 * A035246  Indices of the nonzero terms in expansion of Dirichlet series
 *          Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m=
 *          -12.
 * https://oeis.org/A035246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A035246 – Indices of the nonzero terms in the expansion of
    \prod_p (1-(\Kronecker(-12,p)+1)p^{-s}+ \Kronecker(-12,p)p^{-2s})^{-1}
    For m = -12 a number n has a non‑zero coefficient iff in its
    prime factorisation every prime p ≡ 2 (mod 3) (except 2 and 3)
    occurs to an even exponent.  This code tests that property and
    enumerates the numbers in increasing order.  The nth such number
    is returned.  The routine runs in O(a(n)) time, which for n ≤ 10⁴
    easily stays below the 2‑second limit. */

#include <stdbool.h>
#include <stdint.h>

/* Test whether a positive integer n satisfies the condition that
   all primes p≡2 (mod 3), p≠2,3, appear to an even exponent. */
static bool is_good(uint64_t n)
{
    uint64_t t = n;

    /* Remove factors 2 and 3 – they never cause a zero coefficient.   */
    while ((t & 1ULL) == 0) t >>= 1;          /* divide by 2 */
    while (t % 3ULL == 0) t /= 3ULL;          /* divide by 3 */

    /* Check remaining factors.  We test trial division only over
       primes of the form 6k±1 – the usual 6k±1 sieve.       */
    for (uint64_t d = 5; d <= t / d; d += 6) {
        uint64_t exp = 0;
        while (t % d == 0) {
            t /= d;
            ++exp;
        }
        if (exp && (d % 3ULL) == 2ULL && (exp & 1ULL))
            return false;            /* odd exponent of a forbidden prime */

        uint64_t d2 = d + 2;          /* the partner prime 6k+1 */
        exp = 0;
        while (t % d2 == 0) {
            t /= d2;
            ++exp;
        }
        if (exp && (d2 % 3ULL) == 2ULL && (exp & 1ULL))
            return false;
    }

    /* If a prime > sqrt(original n) remains, its exponent is 1. */
    if (t > 1 && (t % 3ULL) == 2ULL)
        return false;

    return true;
}

/* External linkage: compute the nth non‑zero coefficient index. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t v = 1;      /* candidates start from 1 */

    while (true) {
        if (is_good((uint64_t)v)) {
            ++count;
            if (count == n) return (seq_t)v;
        }
        ++v;
    }
}
