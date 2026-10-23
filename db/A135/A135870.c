/*
 * A135870  G.f. A(x) = (1 + x*A(3x))^2.
 * https://oeis.org/A135870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A135870 – Self-convolution of A135869
    Generating function:  A(x) = (1 + x*A(3x))^2
    Recurrence:
      a[0] = 1
      a[n] = 2*a[n-1]*3^(n-1) + 3^(n-2)*∑_{i=0}^{n-2} a[i]*a[n-2-i]   (n≥2)
      a[1] = 2   (special case of the recurrence)
    Implementation uses dynamic arrays and incremental powers of 3.
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    /* Allocate array for a[0..n] */
    seq_t *arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!arr) {
        return (seq_t)0;               /* Out‑of‑memory: return 0 */
    }

    arr[0] = (seq_t)1;

    /* powers of 3: pow3_prev = 3^(k-1), pow3_prev_prev = 3^(k-2) */
    seq_t pow3_prev        = (seq_t)1;   /* 3^0 for k=1 */
    seq_t pow3_prev_prev   = (seq_t)1;   /* placeholder for 3^-1, used only when k≥2 */

    for (idx_t k = (idx_t)1; k <= n; ++k) {
        seq_t term = (seq_t)0;

        /* Term 2*a[k-1]*3^(k-1) */
        term += (seq_t)2 * arr[k-1] * pow3_prev;

        /* Convolution part, only when k ≥ 2 */
        if (k >= (idx_t)2) {
            seq_t sum = (seq_t)0;
            for (idx_t j = (idx_t)0; j <= k-2; ++j) {
                sum += arr[j] * arr[k-2-j];
            }
            term += pow3_prev_prev * sum;
        }

        arr[k] = term;

        /* Update powers of 3 for next iteration */
        seq_t new_pow3_prev = pow3_prev * (seq_t)3;   /* 3^k */
        pow3_prev_prev = pow3_prev;                  /* 3^(k-1) becomes previous prev */
        pow3_prev = new_pow3_prev;                   /* 3^k becomes current prev */
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
