/*
 * A161798  G.f. satisfies: A(x) = 1/(1 - x/(1 - x*A(x))^3)^2.
 * https://oeis.org/A161798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(2*n-k+1,k)/(n-k+1) * C(n+2*k-1,n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 44,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t sum = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k) {
        /* first binomial: C(2*n-k+1, k) */
        seq_t bin1 = binomial((idx_t)(2 * n - k + 1), k);

        /* second binomial: C(n+2*k-1, n-k) */
        seq_t bin2 = binomial((idx_t)(n + 2 * k - 1), (idx_t)(n - k));

        /* denominator: (n-k+1) */
        seq_t denom = (seq_t)(n - k + 1);

        seq_t term;
        if (sizeof(seq_t) <= sizeof(unsigned long long)) {
            /* use 128‑bit intermediate for native integer types */
            unsigned __int128 prod = (unsigned __int128)(unsigned long long)bin1 *
                                   (unsigned long long)bin2;
            term = (seq_t)(prod / denom);
        } else {
            /* arbitrary‑precision type: rely on built‑in operators */
            term = bin1 * bin2 / denom;
        }

        sum += term;
    }

    return sum;
}
