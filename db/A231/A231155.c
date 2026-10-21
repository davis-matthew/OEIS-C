/*
 * A231155  Largest k such that no sum of digits is divisible by n (zeros not
 *          allowed in the digits of k).
 * https://oeis.org/A231155
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A231470(n) * (10^(n-1)-1)/9
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the largest integer k with n‑1 digits of the maximum
   digit d coprime to n (digits ≠ 0).  The value is  
   d * (10^(n-1)-1)/9.                               */

#include <assert.h>
#include "../utils/pow10.h"

/* Small helper to compute gcd of two unsigned long long values. */
static inline unsigned long long gcd_ull(unsigned long long a, unsigned long long b)
{
    while (b != 0u) {
        unsigned long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    assert(n >= 2);

    /* Find the largest digit d (1-9) such that gcd(n,d)==1. */
    int d = 1;
    for (int cand = 9; cand >= 1; --cand) {
        if (gcd_ull((unsigned long long)n, (unsigned long long)cand) == 1u) {
            d = cand;
            break;
        }
    }

    /* Compute (10^(n-1)-1)/9, the number consisting of n-1 ones. */
    idx_t exp = n - 1u;                /* avoids subtraction underflows */
    seq_t ones = pow10(exp);           /* 10^(n-1) as seq_t */
    ones -= (seq_t)1u;                 /* subtract 1 */
    ones /= (seq_t)9u;                 /* divide by 9 */

    /* Return d multiplied by the ones number. */
    return (seq_t)d * ones;
}
