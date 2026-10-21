/*
 * A070914  Array read by antidiagonals giving number of paths up and left
 *          from (0,0) to (n,kn) where x/y <= k for all intermediate points.
 * https://oeis.org/A070914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T( i, j ) where T(n,k)=binomial(n*(k+1),n)/(n*k+1) Read
 *            the array by antidiagonals: for index n, let s satisfy s*(s+1)/2
 *            <= n < (s+1)*(s+2)/2, offset = n - s*(s+1)/2, then i = offset, j
 *            = s - offset
 * Range:     u8 n <= 32, u16 n <= 72, u32 n <= 145, u64 n <= 317,
 *            u128 n <= 808, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"      /* seq_t gcd(seq_t a, seq_t b) */

/* integer square root of an 128‑bit unsigned integer */
static inline idx_t isqrt128(__int128 x)
{
    idx_t low = 0, high = (idx_t)1 << 63;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 m2 = (__int128)mid * mid;
        if (m2 == x) return mid;
        if (m2 < x) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}

/* compute T(n,k) = binomial(n*(k+1), n)/(n*k+1) without intermediate overflow */
static seq_t T(idx_t n, idx_t k)
{
    seq_t n_seq = (seq_t)n;
    seq_t k_seq = (seq_t)k;

    idx_t N = n * (k + 1);         /* N = n*(k+1) <= 1e8 for n,k <= 1e4 */
    idx_t K = n;                   /* K = n */

    /* allocate numerators [N-K+1 .. N] and denominators [1 .. K] + (n*k+1) */
    seq_t *nums = (seq_t*)calloc(K, sizeof(seq_t));
    seq_t *dens = (seq_t*)calloc(K + 1, sizeof(seq_t));
    if (!nums || !dens) {
        /* allocation failure: return 0 (should not happen for small K) */
        if (nums) free(nums);
        if (dens) free(dens);
        return (seq_t)0;
    }

    seq_t one = (seq_t)1;
    for (idx_t i = 0; i < K; ++i)
        nums[i] = (seq_t)(N - K + 1 + i);   /* N-K+1 .. N */
    for (idx_t i = 0; i < K; ++i)
        dens[i] = (seq_t)(i + 1);           /* 1 .. K */
    dens[K] = (seq_t)(n_seq * k_seq + one); /* n*k+1 */

    /* cancel gcds pairwise to keep intermediates small */
    for (idx_t di = 0; di < K + 1; ++di) {
        seq_t d = dens[di];
        if (d == one) continue;
        if (d == 0) { dens[di] = one; continue; }
        for (idx_t ni = 0; ni < K; ++ni) {
            seq_t g = gcd(nums[ni], d);
            if (g > one) {
                nums[ni] /= g;
                d /= g;
                if (d == one) break;
            }
        }
        dens[di] = d;
    }

    /* After cancellation, all denominators should be 1 */
    /* Multiply remaining numerators */
    seq_t res = one;
    for (idx_t i = 0; i < K; ++i)
        res *= nums[i];

    free(nums);
    free(dens);
    return res;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* find diagonal s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2 */
    __int128 tmp = (__int128)8 * n + 1;
    idx_t sqrt_val = isqrt128(tmp);
    idx_t s = (sqrt_val - 1) / 2;

    idx_t prev = s * (s + 1) / 2;
    idx_t offset = n - prev;

    idx_t i = offset;        /* first coordinate (vertical) */
    idx_t j = s - offset;    /* second coordinate (horizontal) */

    return T(i, j);
}
