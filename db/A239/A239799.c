/*
 * A239799  a(n) = gcd(Sum_{k=1...n} L(k), Product_{j=1...n} L(j)), where L(k)
 *          is the k-th Lucas number.
 * https://oeis.org/A239799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd( sum_{k=1..n} L(k), prod_{k=1..n} L(k) ), where
 *            L(1)=1, L(2)=3, L(k)=L(k-1)+L(k-2) for k≥3
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t
mod_mul(seq_t a, seq_t b, seq_t mod)
{
    seq_t res = 0;
    a %= mod;
    while (b > 0) {
        if (b & 1) {
            res += a;
            if (res >= mod) res -= mod;
        }
        a += a;
        if (a >= mod) a -= mod;
        b >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* First compute the sum S = Σ_{k=1..n} L(k). */
    seq_t S = 0;
    seq_t L1 = 1, L2 = 3;
    if (n >= 1) S += L1;          /* k = 1 */
    if (n >= 2) {
        S += L2;                  /* k = 2 */
        for (idx_t k = 3; k <= n; ++k) {
            seq_t Lk = L1 + L2;   /* L(k) = L(k-1)+L(k-2) */
            S += Lk;
            L1 = L2;
            L2 = Lk;
        }
    }

    /* Now compute product modulo S. */
    seq_t prod = 1;
    L1 = 1; L2 = 3;                /* reset Lucas recurrence */
    if (n >= 1) prod = mod_mul(prod, L1, S);   /* k = 1 */
    if (n >= 2) prod = mod_mul(prod, L2, S);   /* k = 2 */
    for (idx_t k = 3; k <= n; ++k) {
        seq_t Lk = L1 + L2;
        prod = mod_mul(prod, Lk, S);
        L1 = L2;
        L2 = Lk;
    }

    return gcd(S, prod);
}
