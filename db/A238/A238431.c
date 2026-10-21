/*
 * A238431  Expansion of F(x) where F(x) = 1 + x / (1 - x * F(x^2)^2 ).
 * https://oeis.org/A238431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 36, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A238431
 *  Recurrence derived from the functional equation
 *  F(x) = 1 + x / (1 - x*F(x^2)^2)
 *  Let a(n) be the coefficient of x^n in F(x).
 *  Define: g_{2t+1} = b(t) where
 *          b(t) = sum_{i=0..t} a(i)*a(t-i)
 *  The auxiliary series H(x) = 1/(1 - G(x)) satisfies
 *  H = 1 + G*H
 *  with H_0 = 1 and for m >= 1:
 *          H_m = sum_{i=1..m | i odd} b((i-1)/2) * H_{m-i}
 *  Then a(n) = 1  (n==0 or n==1) and for n >= 2:
 *          a(n) = H_{n-1}
 *
 *  The implementation computes a[0..n] together with b[0..n] and H[0..n-1]
 *  in a single linear pass using only integer operations.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)1;

    /* arrays of size n+2 to store a[0..n], b[0..n], h[0..n-1] */
    size_t sz = (size_t)n + 2;
    seq_t *a_arr = (seq_t*)malloc(sz * sizeof(seq_t));
    seq_t *b_arr = (seq_t*)malloc(sz * sizeof(seq_t));
    seq_t *h_arr = (seq_t*)malloc((size_t)n * sizeof(seq_t));

    /* initialise base values */
    a_arr[0] = (seq_t)1;
    a_arr[1] = (seq_t)1;

    /* b[0] = a0*a0 */
    b_arr[0] = a_arr[0] * a_arr[0];
    /* b[1] = a0*a1 + a1*a0 */
    b_arr[1] = a_arr[0] * a_arr[1] + a_arr[1] * a_arr[0];

    /* H_0 = 1 */
    h_arr[0] = (seq_t)1;

    /* compute a[2..n] sequentially */
    for (idx_t m = 1; m <= n - 1; ++m) {
        seq_t sum = (seq_t)0;
        /* i goes over odd indices <= m */
        for (idx_t i = 1; i <= m; i += 2) {
            idx_t t = (i - 1) / 2;          /* t = (i-1)/2 */
            sum += b_arr[t] * h_arr[m - i];
        }
        h_arr[m] = sum;
        a_arr[m + 1] = sum;                /* a[n] = H_{n-1} */

        /* compute b[m+1] for future use */
        idx_t idx = m + 1;
        if (idx <= n) {
            seq_t val = (seq_t)0;
            for (idx_t p = 0; p <= idx; ++p) {
                val += a_arr[p] * a_arr[idx - p];
            }
            b_arr[idx] = val;
        }
    }

    seq_t result = a_arr[n];

    free(a_arr);
    free(b_arr);
    free(h_arr);

    return result;
}
