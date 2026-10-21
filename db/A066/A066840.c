/*
 * A066840  Sum of positive integers k where k <= n/2 and gcd(k,n) = 1.
 * https://oeis.org/A066840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{1 ≤ k ≤ ⌊n/2⌋, gcd(k,n)=1} k This implementation uses
 *            inclusion–exclusion over the distinct prime factors of n. The
 *            code is fully generic for any seq_t and idx_t types provided by
 *            the build
 * Range:     u8 n <= 46, u16 n <= 726, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* floor(n/2) */
    idx_t half = n / 2;
    if (half == 0) return 0;

    /* collect distinct prime factors of n */
    uint64_t primes[64];
    size_t cnt = 0;
    uint64_t tmp = (uint64_t)n;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            primes[cnt++] = p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) {
        primes[cnt++] = tmp;
    }

    seq_t res = 0;

    /* iterate over all subsets of distinct prime factors */
    uint64_t subsets = 1ULL << cnt;      /* cnt <= 64; for n≤10000 cnt≤5   */
    for (uint64_t mask = 0; mask < subsets; ++mask) {
        seq_t d = 1;             /* product of primes in the subset   */
        int bits = 0;            /* number of selected primes         */
        for (size_t i = 0; i < cnt; ++i) {
            if (mask & (1ULL << i)) {
                d *= (seq_t)primes[i];
                ++bits;
            }
        }
        /* maximum multiple of d not exceeding half   */
        idx_t m = half / (idx_t)d;
        if (m == 0) continue;

        /* sum of first m integers: m*(m+1)/2   */
        seq_t sum_m = (seq_t)m * (m + 1) / 2;
        seq_t term  = d * sum_m;          /* sum of multiples of d up to half */

        if ((bits & 1) == 0)  /* even number of primes: add   */
            res += term;
        else                  /* odd number of primes: subtract */
            res -= term;
    }

    return res;
}
