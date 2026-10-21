/*
 * A065734  Largest square <= sigma(n).
 * https://oeis.org/A065734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = largest square ≤ σ(n) where σ(n) is the sum of divisors
 *            of n. The sequence is a(n) = A048760(σ(n))
 * Range:     u8 n <= 20, u16 n <= 818, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute σ(n) exactly using 64‑bit arithmetic.  The value stays
   within 64 bits for all n up to 100 000 (more than enough for the
   tests).  The result is then safely cast to seq_t. */
static inline uint64_t sigma_uint64(idx_t n)
{
    uint64_t sum = 1;          /* divisor 1 */
    uint64_t rem = n;
    /* handle factor 2 */
    uint64_t exp = 0;
    while ((rem & 1ULL) == 0) {
        rem >>= 1;
        ++exp;
    }
    if (exp > 0) {
        /* term = 2^(exp+1) - 1 */
        uint64_t term = (1ULL << (exp + 1)) - 1;
        sum *= term;
    }
    /* odd factors */
    for (uint64_t p = 3; p * p <= rem; p += 2) {
        if (rem % p == 0) {
            exp = 0;
            while (rem % p == 0) {
                rem /= p;
                ++exp;
            }
            /* term = (p^(exp+1)-1)/(p-1) */
            uint64_t p_pow = 1;
            for (uint64_t i = 0; i <= exp; ++i) p_pow *= p;
            uint64_t term = (p_pow - 1) / (p - 1);
            sum *= term;
        }
    }
    if (rem > 1) {
        /* remaining prime factor with exponent 1 */
        sum *= (rem + 1);
    }
    return sum;
}

/* Integer square root of a seq_t value by binary search.  Works
   for any unsigned seq_t (8, 16, 32, 64 or 128 bits). */
static inline seq_t isqrt_seq(seq_t x)
{
    seq_t lo = 0;
    seq_t hi = x;
    seq_t ans = 0;
    while (lo <= hi) {
        seq_t mid = lo + ((hi - lo) >> 1);
        seq_t sq = mid * mid;
        if (sq == x) {
            return mid;
        }
        if (sq < x) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;  /* undefined for 0, safe default */

    /* σ(n) in 64‑bit */
    uint64_t sigma = sigma_uint64(n);

    /* largest square ≤ σ(n) */
    seq_t root = isqrt_seq((seq_t)sigma);
    return root * root;
}
