/*
 * A226746  Numbers n such that x^2 = 1 has more than two solutions in the
 *          Gaussian integers modulo n.
 * https://oeis.org/A226746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the n‑th positive integer such that in
   the Gaussian integers modulo that integer the equation x² = 1
   has more than two solutions.  */
/* The criterion is:
     * n divisible by 4  →  true
     * n divisible by 2 but not 4  → true unless n == 2
     * otherwise (odd n):  true iff
            (n has a prime factor ≡ 1 (mod 4))  or
            (n has at least two distinct prime factors ≡ 3 (mod 4))  */
#include <stdint.h>
#include <stdbool.h>

static inline bool has_extra_solutions(uint64_t m)
{
    /* 1,2,3 never satisfy */
    if (m <= 3) return false;

    /* Even numbers */
    if ((m & 1ULL) == 0) {
        if (m % 4 == 0)             /* 2ⁿ with n ≥ 2  */
            return true;
        /* m == 2 is the only even number without extra solutions */
        return m != 2;
    }

    /* Odd numbers */
    uint64_t t = m;
    int inert_cnt = 0;            /* count of distinct primes ≡ 3 (mod 4) */

    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            if (p % 4 == 1)        /* a “split” prime,  x² = 1 has 4 ways   */
                return true;
            ++inert_cnt;           /* distinct inert prime found          */
            while (t % p == 0)     /* remove all powers of p              */
                t /= p;
            if (inert_cnt >= 2)    /* two distinct inert primes give 4 ways */
                return true;
        }
    }

    /* remaining factor > 1 */
    if (t > 1) {
        if (t % 4 == 1)            /* last prime is split */
            return true;
        ++inert_cnt;
        if (inert_cnt >= 2)
            return true;
    }

    return false;
}

seq_t a(idx_t n)
{
    idx_t cnt = 0;            /* number of found terms   */
    uint64_t m   = 2;         /* candidate integer      */

    while (cnt < n) {
        if (has_extra_solutions(m)) {
            ++cnt;
            if (cnt == n)
                return (seq_t)m;
        }
        ++m;
    }
    /* Control never reaches here for valid inputs */
    return (seq_t)0;
}
