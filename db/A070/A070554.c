/*
 * A070554  Number of positive integers, k, where k <= 2n+1 and gcd(k, 2n+1) =
 *          gcd(k+1, 2n+1) = 1.
 * https://oeis.org/A070554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p^e‖(2n+1)} (p-2) · p^{e-1}
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* count of k (1 ≤ k ≤ 2n+1) with gcd(k,2n+1)=gcd(k+1,2n+1)=1 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    /* M = 2n + 1  (odd for all n ≥ 0) */
    uint64_t M = ((uint64_t)n) * 2ULL + 1ULL;

    /* trivial case M = 1 */
    if (M <= 1) return result;

    uint64_t tmp = M;
    uint64_t p, e, i;

    /* handle factor 3 first (skip 2 as M is odd) */
    p = 3;
    e = 0;
    while (tmp % p == 0) {
        tmp /= p;
        e++;
    }
    if (e) {
        seq_t factor = (seq_t)(p - 2);      /* (p-2) */
        for (i = 1; i < e; ++i) {          /* multiply p^{e-1} */
            factor *= (seq_t)p;
        }
        result *= factor;
    }

    /* trial division by odd primes starting from 5 */
    for (p = 5; p * p <= tmp; p += 2) {
        e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            e++;
        }
        if (e) {
            seq_t factor = (seq_t)(p - 2);  /* (p-2) */
            for (i = 1; i < e; ++i) {
                factor *= (seq_t)p;
            }
            result *= factor;
        }
    }

    /* any remaining prime factor > sqrt(tmp) */
    if (tmp > 1) {
        seq_t factor = (seq_t)(tmp - 2);    /* (p-2) with e = 1 */
        result *= factor;
    }

    return result;
}
