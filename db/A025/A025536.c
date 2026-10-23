/*
 * A025536  a(n) = (1/C(n,0) - 1/C(n,1) + ... + d/C(n,k))*L, where d =
 *          (-1)^k,k = [ n/2 ], L = LCM{C(n,0), C(n,1),..., C(n,n)}.
 * https://oeis.org/A025536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Σ_{i=0}^{⌊n/2⌋} (-1)^i / C(n,i)) * LCM{C(n,i)}
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 27, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* 1. Compute L = LCM of all binomial coefficients C(n,i). */
    seq_t L = 1;
    seq_t comb = 1;                 /* C(n,0) = 1 */
    for (idx_t i = 1; i <= n; ++i) {
        comb = comb * (seq_t)(n - i + 1) / (seq_t)i;   /* C(n,i) from C(n,i-1) */
        seq_t g = gcd(L, comb);
        L = L / g * comb;                 /* lcm(L, comb) */
    }

    /* 2. Accumulate the alternating partial sum using the common
       denominator L.  The partial sums never go negative, so unsigned
       arithmetic is safe. */
    idx_t k = n / 2;
    seq_t sum = 0;
    comb = 1;                 /* reset to C(n,0) */
    long long sign = 1;       /* +1 for i=0, then alternates */

    for (idx_t i = 0; i <= k; ++i) {
        seq_t term = L / comb;          /* integer since L is lcm */
        if (sign > 0) sum += term;
        else          sum -= term;
        sign = -sign;

        /* Prepare C(n,i+1) if we are not at the last required term. */
        if (i < k) {
            comb = comb * (seq_t)(n - i) / (seq_t)(i + 1);
        }
    }

    return sum;     /* a(n) */
}
