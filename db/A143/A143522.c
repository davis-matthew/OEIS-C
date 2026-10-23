/*
 * A143522  a(n) = n-fold Dumont operator of x evaluated at x=1, y=1, z=2.
 * https://oeis.org/A143522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A143522:  a(n) = D^n x evaluated at (x=1,y=1,z=2),
   where D = y*z*∂/∂x + z*x*∂/∂y + x*y*∂/∂z.
   Recurrence derived from the exponential generating function:
      A(x) * (3*cosh(√3 x) - 2√3 sinh(√3 x)) = 3
   gives
      a_0 = 1,
      a_n = -(1/3) * Σ_{j=0}^{n-1} α_{n-j} * a_j * binom(n,j)   (n ≥ 1),
   where
      α_m =  3^{m/2+1}                 if m even,
      α_m = -2 * 3^{(m+1)/2}           if m odd.
   The implementation uses 128‑bit signed arithmetic for intermediates
   and stores the results in seq_t.  It runs in O(n²) time and has
   negligible memory usage (≈ 8 * n bytes for a(n)). */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Pre‑compute powers of 3 up to floor(n/2)+1. */
    size_t kmax = (size_t)(n / 2);
    size_t pow_size = kmax + 2;                  /* need index up to kmax+1 */
    seq_t *pow3 = (seq_t *)malloc(pow_size * sizeof(seq_t));
    if (!pow3) return 0; /* allocation failure – not expected in tests */
    pow3[0] = (seq_t)1;
    for (size_t i = 1; i < pow_size; ++i)
        pow3[i] = pow3[i - 1] * (seq_t)3;

    /* Array for sequence values. */
    seq_t *a_arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!a_arr) { free(pow3); return 0; }
    a_arr[0] = (seq_t)1;

    for (idx_t k = 1; k <= n; ++k)
    {
        __int128 sum = 0;   /* accumulates Σ α_{k-j} * a_j * binom(k,j) */

        __int128 binom = 1; /* binom(k,0) */
        for (idx_t j = 0; j < k; ++j)
        {
            idx_t m = k - j;                     /* m = k-j  */
            __int128 alpha;
            if ((m & 1) == 0)                    /* m even */
            {
                idx_t kk = m / 2;                /* kk = k for α_{2k} */
                alpha = (__int128)pow3[kk + 1];  /* α_m = 3^{kk+1} */
            }
            else                                /* m odd */
            {
                idx_t kk = (m - 1) / 2;          /* kk = (m-1)/2  */
                alpha = -(__int128)2 * (__int128)pow3[kk + 1];
            }

            /* term = α_m * a_j * binom(k,j) */
            __int128 term = alpha * (__int128)a_arr[j] * binom;
            sum += term;

            /* update binom for next j: binom(k,j+1) = binom(k,j)*(k-j)/(j+1) */
            binom = binom * (k - j) / (j + 1);
        }

        /* a_k = -sum / 3  (sum is negative). Cast to seq_t. */
        a_arr[k] = (seq_t)(-sum / ( __int128)3);
    }

    seq_t result = a_arr[n];
    free(a_arr);
    free(pow3);
    return result;
}
