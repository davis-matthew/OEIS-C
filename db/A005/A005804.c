/*
 * A005804  Number of phylogenetic rooted trees with n labels.
 * https://oeis.org/A005804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   E.g.f. f(x) satisfies 2*f(x) - exp(f(x)) = exp(x) - 2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   alloc_zero, binom, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

seq_t a(idx_t n) {
    seq_t *f = alloc_zero(n + 1);
    seq_t *h = alloc_zero(n + 1);
    
    f[1] = 1;
    h[0] = 1;
    
    for (idx_t m = 1; m <= n; m++) {
        seq_t sum = 0;
        for (idx_t k = 0; k < m - 1; k++) {
            seq_t binom_coeff = binom(m - 1, k);
            sum += binom_coeff * f[k + 1] * h[m - 1 - k];
        }
        h[m] = 1 + 2 * sum;
        f[m] = (1 + h[m]) / 2;
    }
    
    seq_t result = f[n];
    release(f, n + 1);
    release(h, n + 1);
    return result;
}
