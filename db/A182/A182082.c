/*
 * A182082  Number of pairs, (x,y), with x >= y, whose LCM does not exceed n.
 * https://oeis.org/A182082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (d(k^2)+1)/2, where d is the divisor
 *            function
 * Range:     u8 n <= 49, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a simple spf (smallest prime factor) sieve and
   computes the divisor‑count of k^2 directly for each k.  The algorithm is
   O(n log n) and easily fits within the 2‑second limit for n≈10000. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* out of offset; not expected */

    /*--- Build smallest prime factor array up to n ---------------------------------*/
    uint64_t *spf = (uint64_t*)malloc((n + 1) * sizeof(uint64_t));
    if (!spf) abort();         /* Allocation failure is fatal for this helper.   */
    for (idx_t i = 0; i <= n; ++i)
        spf[i] = 0;

    if (n >= 1) spf[1] = 1;        /* 1 has no prime factor */
    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {          /* i is prime */
            spf[i] = i;
            if ((idx_t)i * (idx_t)i <= n) {
                for (idx_t j = i * i; j <= n; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    /*--- Accumulate the sum of (d(k^2)+1)/2 ----------------------------------------*/
    seq_t result = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t dsq = (seq_t)1;      /* product of (2*exp + 1) over prime factors of k */
        idx_t temp = k;
        while (temp > 1) {
            idx_t p = spf[temp];
            idx_t cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            dsq *= (seq_t)(2 * cnt + 1);
        }
        result += (dsq + (seq_t)1) / (seq_t)2;
    }

    free(spf);
    return result;
}
