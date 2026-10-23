/*
 * A128758  Expansion of q^(-1/3) * (eta(q^3) / eta(q))^4 in powers of q.
 * https://oeis.org/A128758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n ≥ 1: a(n) = (1/n) * Σ_{k=1}^{n} L(k) * a(n‑k)
 *            where L(k) = Σ_{d|k} d * b(d) and b(d) = 4 if d mod 3 ≠ 0, else
 *            0. (Euler transform of the periodic sequence 4,4,0,4,4,0,…) The
 *            recurrence gives the expansion of q^(‑1/3)*(η(q³)/η(q))⁴
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 44, u64 n <= 149, u128 n <= 529,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate arrays */
    seq_t *A = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!A) return 0;           /* fallback if allocation fails */

    uint32_t *L = (uint32_t *)malloc((size_t)(n + 1) * sizeof(uint32_t));
    if (!L) { free(A); return 0; }

    /* Initialize arrays */
    A[0] = 1;
    for (idx_t i = 1; i <= n; ++i) L[i] = 0;

    /* Precompute L(k) = Σ_{d|k} d* b(d) */
    for (idx_t d = 1; d <= n; ++d) {
        if (d % 3 != 0) {
            uint32_t add = (uint32_t)(4 * d);   /* 4*d fits in 32 bits for n ≤ 10000 */
            for (idx_t m = d; m <= n; m += d) {
                L[m] += add;
            }
        }
    }

    /* Compute a(n) by the recurrence */
    for (idx_t i = 1; i <= n; ++i) {
#ifdef __cplusplus
        /* C++17: use compile‑time branch */
        if constexpr (sizeof(seq_t) <= 8) {
            unsigned __int128 sum = 0;
            for (idx_t k = 1; k <= i; ++k) {
                sum += (unsigned __int128)A[i - k] * (unsigned __int128)L[k];
            }
            A[i] = (seq_t)(sum / i);
        } else {
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k) {
                sum += A[i - k] * (seq_t)L[k];
            }
            A[i] = sum / i;
        }
#else
        /* C11: always use 128‑bit intermediate if allowed */
        unsigned __int128 sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            sum += (unsigned __int128)A[i - k] * (unsigned __int128)L[k];
        }
        A[i] = (seq_t)(sum / i);
#endif
    }

    seq_t result = A[n];
    free(L);
    free(A);
    return result;
}
