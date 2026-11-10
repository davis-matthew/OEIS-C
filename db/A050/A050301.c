/*
 * A050301  Matrix 7th power of partition triangle A008284.
 * https://oeis.org/A050301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (T^7)(r,k) where n = r(r-1)/2 + k, 1<=k<=r, T =
 *            lower-triangular matrix of A008284: T(n,k)=T(n-1,k-1)+T(n-k,k),
 *            T(n,n)=1, T(n,1)=1
 * Range:     u8 n <= 10, u16 n <= 55, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/mem.h"

static void A050301_mul(const seq_t *A, const seq_t *B, seq_t *C, size_t w)
{
    for (size_t i = 1; i < w; i++)
        for (size_t j = 1; j <= i; j++) {
            seq_t s = 0;
            for (size_t k = j; k <= i; k++)
                s += A[i * w + k] * B[k * w + j];
            C[i * w + j] = s;
        }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    size_t r = 1;
    while ((idx_t)r * (idx_t)(r + 1) / 2 < n) r++;
    size_t k = (size_t)(n - (idx_t)r * (idx_t)(r - 1) / 2);
    size_t w = r + 1;
    size_t sz = w * w;

    seq_t *P  = mem_new(seq_t, sz);
    seq_t *P2 = mem_new(seq_t, sz);
    seq_t *P4 = mem_new(seq_t, sz);
    seq_t *P3 = mem_new(seq_t, sz);
    seq_t *P7 = mem_new(seq_t, sz);

    for (size_t i = 1; i <= r; i++) {
        P[i * w + i] = 1;
        for (size_t j = 1; j < i; j++)
            P[i * w + j] = P[(i - 1) * w + (j - 1)] + P[(i - j) * w + j];
    }
    A050301_mul(P, P, P2, w);
    A050301_mul(P2, P, P3, w);
    A050301_mul(P2, P2, P4, w);
    A050301_mul(P4, P3, P7, w);

    seq_t res = P7[r * w + k];
    mem_free(P); mem_free(P2); mem_free(P4); mem_free(P3); mem_free(P7);
    return res;
}
