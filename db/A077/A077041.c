/*
 * A077041  Primes p such that abs(A077039(A049084(p))) > p.
 * https://oeis.org/A077041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime p with |A077039(pi(p))| > p, where A077039(k)
 *            = sum over primes q=prime(2..k) of (+q if q==1 mod 4, -q if q==3
 *            mod 4); found by sieve and running sum
 * Range:     u8 n <= 24, u16 n <= 5944, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9965 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    size_t limit = 2048;
    for (;;) {
        unsigned char *comp = (unsigned char *)calloc(limit + 1, 1);
        if (!comp) return (seq_t)0;
        long long S = 0;
        idx_t cnt = 0;
        for (size_t p = 2; p <= limit; ++p) {
            if (comp[p]) continue;
            if (p <= limit / p) {
                for (size_t m = p * p; m <= limit; m += p) comp[m] = 1;
            }
            if (p > 2) {
                if ((p & 3) == 1) S += (long long)p;
                else S -= (long long)p;
            }
            long long as = S < 0 ? -S : S;
            if (as > (long long)p) {
                ++cnt;
                if (cnt == n) {
                    free(comp);
                    return (seq_t)p;
                }
            }
        }
        free(comp);
        limit *= 2;
    }
}
