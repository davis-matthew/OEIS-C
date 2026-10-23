/*
 * A108452  Number of paths from (0,0) to (3n,0) that stay in the first
 *          quadrant (but may touch the horizontal axis), consisting of steps
 *          u=(2,1),U=(1,2), or d=(1,-1) and having no pyramids of the first
 *          kind (a pyramid of the first kind is a sequence u^pd^p for some
 *          positive integer p, starting at the x-axis).
 * https://oeis.org/A108452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This implementation follows the generating‑function approach from the OEIS entry:
   G(z) = (1 – z) / (1 – z(1–z)A(1 + A)),     with  A = 1 + zA² + zA³.
   The coefficients of A are computed in a linear loop (quadratic total time),
   then the auxiliary series F = A + A², the series H = zF – z²F, and finally
   the series S = 1 + H·S are formed by convolution.  The desired term is
   a(n) = S[n] – S[n‑1].
   The whole algorithm is O(n²) arithmetic operations and uses only integer
   arithmetic (no floating point).  Memory usage is linear in n.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Allocation helpers that work in both C and C++ */
#if defined(__cplusplus)
#  define ALLOC(type, sz) new type[sz]
#  define FREE(ptr)       delete[] ptr
#else
#  define ALLOC(type, sz) ((type *)malloc((size_t)(sz)*sizeof(type)))
#  define FREE(ptr)       free(ptr)
#endif

/* --------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;                     /* a(0) = 1 */

    /* Allocate arrays ----------------------------------------------------- */
    idx_t  sizeA   = n + 1;
    idx_t  sizeF   = n + 1;
    idx_t  sizeH   = n + 1;
    idx_t  sizeS   = n + 1;
    idx_t  sizeC2  = 2 * n + 1;

    seq_t *aA  = ALLOC(seq_t, sizeA);
    seq_t *conv2 = ALLOC(seq_t, sizeC2);
    seq_t *f    = ALLOC(seq_t, sizeF);
    seq_t *h    = ALLOC(seq_t, sizeH);
    seq_t *s    = ALLOC(seq_t, sizeS);

    /* initialise arrays ----------------------------------------------------- */
    for (idx_t i = 0; i < sizeC2; ++i) conv2[i] = (seq_t)0;
    for (idx_t i = 0; i < sizeF;  ++i) f[i]    = (seq_t)0;
    for (idx_t i = 0; i < sizeH;  ++i) h[i]    = (seq_t)0;
    for (idx_t i = 0; i < sizeS;  ++i) s[i]    = (seq_t)0;

    /* aA[0] = 1, conv2[0] = aA[0]*aA[0] = 1 ---------------------------- */
    aA[0]    = (seq_t)1;
    conv2[0] = (seq_t)1;

    /* Compute the coefficients of A (aA) ----------------------------------- */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t conv2_k_minus1 = conv2[k-1];

        /* sum_{i=0}^{k-1} aA[i] * conv2[k-1-i] */
        seq_t sum = (seq_t)0;
        for (idx_t i = 0; i < k; ++i)
            sum += aA[i] * conv2[k-1-i];

        aA[k] = conv2_k_minus1 + sum;

        /* Update conv2 with the new coefficient aA[k]  */
        for (idx_t j = 0; j < k; ++j) {
            idx_t idx = k + j;                       /* target index */
            conv2[idx] += aA[j] * aA[k] * (seq_t)2;   /* (j,k) and (k,j) */
        }
        /* contribution of pair (k,k) */
        conv2[2*k] += aA[k] * aA[k];
    }

    /* Compute F = A + A² --------------------------------------------------- */
    for (idx_t i = 0; i <= n; ++i) {
        f[i] = aA[i] + conv2[i];   /* conv2[i] already contains A²[i]   */
    }

    /* Compute H = zF – z²F ------------------------------------------------- */
    /* h[1] = f[0] , h[k] = f[k-1] – f[k-2] for k >= 2                       */
    h[1] = f[0];
    for (idx_t k = 2; k <= n; ++k)
        h[k] = f[k-1] - f[k-2];

    /* Compute S by solving S = 1 + H·S ------------------------------------- */
    s[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k)               /* convolution term */
            sum += h[k] * s[i-k];
        s[i] = sum;
    }

    /* Final term a(n) = S[n] – S[n-1] ------------------------------------- */
    seq_t result = s[n] - s[n-1];

    /* cleanup --------------------------------------------------------------- */
    FREE(aA);
    FREE(conv2);
    FREE(f);
    FREE(h);
    FREE(s);

    return result;
}
