/*
 * A023820  Sum of exponents in prime-power factorization of C(3n,n-1).
 * https://oeis.org/A023820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p prime} ( ⌊3n/p^k⌋ - ⌊(n-1)/p^k⌋ - ⌊(2n+1)/p^k⌋ )
 *            over all k ≥ 1
 * Range:     u8 n <= 852, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/build_spf.h"
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1, but guard anyway */

    /* Cache the smallest prime factor array up to 3n */
    static idx_t *spf = NULL;
    static idx_t spf_limit = 0;

    idx_t limit = 3 * n;
    if (limit > spf_limit) {
        if (spf) free(spf);          /* discard old cache */
        build_spf(limit, &spf);
        spf_limit = limit;
    }

    seq_t total = 0;

    for (idx_t p = 2; p <= limit; ++p) {
        if (spf[p] != p) continue;   /* skip non‑primes */

        seq_t e = 0;
        idx_t pk = p;
        while (pk <= limit) {
            e +=   (seq_t)(3 * n)   / pk;
            e -=   (seq_t)(n - 1)   / pk;
            e -=   (seq_t)(2 * n + 1) / pk;
            if (pk > limit / p) break;   /* prevent overflow */
            pk *= p;
        }
        total += e;
    }

    return total;
}
