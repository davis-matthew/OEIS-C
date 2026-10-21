/*
 * A211056  Number of 2 X 2 nonsingular matrices having all terms in
 *          {1,...,n}.
 * https://oeis.org/A211056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^4 – ∑_{k=1}^n g(k) ⋅ ⌊n/k⌋², where g(1)=1 and
 *            g(k)=2⋅φ(k) for k>1 (φ is Euler's totient)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset starts at 1 */
    /* Allocate phi array */
    uint64_t *phi = (uint64_t*)malloc((n + 1) * sizeof(uint64_t));
    if (!phi) return 0;                      /* allocation failure */

    for (idx_t i = 0; i <= n; ++i) phi[i] = i;
    for (idx_t p = 2; p <= n; ++p) {
        if (phi[p] == p) {                   /* p is prime */
            for (idx_t m = p; m <= n; m += p)
                phi[m] -= phi[m] / p;
        }
    }

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        uint64_t g = (k == 1) ? 1ULL : 2ULL * phi[k];
        seq_t factor = (seq_t)n / (seq_t)k;          /* ⌊n/k⌋ */
        seq_t add    = (seq_t)g * factor * factor;  /* g * factor² */
        sum += add;
    }

    free(phi);
    seq_t total = (seq_t)n * (seq_t)n * (seq_t)n * (seq_t)n;  /* n⁴ */
    return total - sum;                       /* nonsingular count */
}
