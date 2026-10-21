/*
 * A092915  a(n) = largest k such that n divides (n-1)!/k!, or 0 if no such k
 *          exists (i.e., if n is prime).
 * https://oeis.org/A092915
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = largest k with n | (n-1)! / k!, or 0 if such k does not exist.
 * This implementation factors n, then for each prime factor p of n determines
 * the maximal k that still leaves the exponent of p in the quotient above or
 * equal to the exponent of p in n. The final k is the minimum over all prime
 * divisors, and 0 is returned if no such k exists (e.g., n is prime).
 */

#include <stdint.h>

/* helper: exponent of prime p in m! */
static uint64_t fact_exp(uint64_t m, uint64_t p)
{
    uint64_t exp = 0;
    while (m) {
        m /= p;
        exp += m;
    }
    return exp;
}

seq_t a(idx_t n)
{
    if (n < 6) return 0;               /* sequence offset is 6 */
    uint64_t N = (uint64_t)n;
    uint64_t minK = (uint64_t)-1;     /* will be minimized */

    /* factor n */
    uint64_t temp = N;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p) continue;
        uint64_t vp = 0;
        while (temp % p == 0) { temp /= p; ++vp; }

        uint64_t totalExp = fact_exp(N - 1, p);
        int64_t needed = (int64_t)totalExp - (int64_t)vp;
        if (needed < 0) {          /* impossible for this prime */
            return 0;
        }

        /* binary search for largest k with exp(k!) <= needed */
        uint64_t lo = 0, hi = N - 1, best = 0;
        while (lo <= hi) {
            uint64_t mid = (lo + hi) >> 1;
            uint64_t expK = fact_exp(mid, p);
            if (expK <= (uint64_t)needed) {
                best = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
        if (best < minK) minK = best;
    }

    /* remaining prime factor > sqrt(N) */
    if (temp > 1) {
        uint64_t p = temp;
        uint64_t vp = 1;             /* temp is prime */
        uint64_t totalExp = fact_exp(N - 1, p);
        int64_t needed = (int64_t)totalExp - (int64_t)vp;
        if (needed < 0) {            /* impossible */
            return 0;
        }
        uint64_t lo = 0, hi = N - 1, best = 0;
        while (lo <= hi) {
            uint64_t mid = (lo + hi) >> 1;
            uint64_t expK = fact_exp(mid, p);
            if (expK <= (uint64_t)needed) {
                best = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
        if (best < minK) minK = best;
    }

    if (minK == (uint64_t)-1) return 0;
    return (seq_t)minK;
}
