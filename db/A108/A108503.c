/*
 * A108503  Number of ordered factorizations of 4*n into even numbers.
 * https://oeis.org/A108503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{v2(n)+2} ( C(v2(n)+1, k-1) * ∏_{p odd}
 *            C(vp(n)+k-1, k-1) ) where vp(n) is the exponent of the odd prime
 *            p in n
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binom_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binom_small.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* safety: domain starts at 1 */

    /* Count exponent of 2 in n */
    idx_t e = 0;
    idx_t tmp = n;
    while ((tmp & 1) == 0) {
        ++e;
        tmp >>= 1;
    }

    /* Factorize the odd part of n, storing exponents */
    idx_t temp = tmp;                /* odd part */
    idx_t exps[64];
    size_t cnt = 0;
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t ei = 0;
            while (temp % p == 0) {
                ++ei;
                temp /= p;
            }
            exps[cnt++] = ei;
        }
    }
    if (temp > 1) {
        exps[cnt++] = 1; /* remaining prime factor */
    }

    /* Sum contributions from all possible numbers of factors k */
    seq_t total = 0;
    for (idx_t k = 1; k <= e + 2; ++k) {
        seq_t ways = binom_small(e + 1, k - 1);   /* distribution of 2‑exponent */
        for (size_t i = 0; i < cnt; ++i) {
            ways *= binom_small(exps[i] + k - 1, k - 1);
        }
        total += ways;
    }

    return total;
}
