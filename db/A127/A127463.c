/*
 * A127463  a(0)=1. a(n) = number of earlier terms a(k), 0<=k<=n-1, such that
 *          (k+a(k)) is coprime to n.
 * https://oeis.org/A127463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n ≥ 1, a(n) = #{k: 0 ≤ k < n and gcd(k + a(k), n)
 *            = 1}
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Allocate array to hold a(0)…a(n) */
    size_t len = (size_t)n + 1;
    seq_t *seq;

#ifdef __cplusplus
    seq = new seq_t[len];
#else
    seq = (seq_t*)malloc(len * sizeof(seq_t));
#endif
    if (!seq) return (seq_t)0;   /* allocation failure */

    seq_t const one = (seq_t)1;
    seq[0] = one;                /* base case */

    for (idx_t m = 1; m <= n; ++m) {
        seq_t cnt = 0;
        seq_t m_seq = (seq_t)m;
        for (idx_t k = 0; k < m; ++k) {
            seq_t sum = (seq_t)k + seq[k];
            if (gcd(sum, m_seq) == one)
                ++cnt;
        }
        seq[m] = cnt;
    }

    seq_t result = seq[n];

#ifdef __cplusplus
    delete[] seq;
#else
    free(seq);
#endif
    return result;
}
