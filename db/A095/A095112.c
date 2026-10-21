/*
 * A095112  a(n) is the sum of n/k over all prime powers k > 1 which divide n.
 * https://oeis.org/A095112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p^e || n, e>=1} Sum_{i=1..e} n / p^i
 * Range:     u8 n <= 215, u16 n <= 9273, u32 n <= 9273, u64 n <= 9273,
 *            u128 n <= 9273, big any n
 * Helpers:   factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here the sum is over prime powers dividing n. */

#include <stdint.h>
#include "../utils/factor_uint64.h"

seq_t a(idx_t n) {
    if (n <= 1) return 0;          /* a(1) = 0, and trivial for n=0 */

    uint64_t fac[20];
    int exp[20];
    int cnt;
    factor_uint64(n, fac, exp, &cnt);

    seq_t total = 0;
    seq_t nseq = (seq_t)n;

    for (int idx = 0; idx < cnt; ++idx) {
        uint64_t p = fac[idx];
        seq_t denom = 1;
        for (int e = 1; e <= exp[idx]; ++e) {
            denom *= (seq_t)p;             /* denom = p^e */
            total += nseq / denom;          /* add n / p^e */
        }
    }
    return total;
}
