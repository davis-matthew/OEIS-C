/*
 * A127770  a(n) = A127768(2n+1). A bisection of the row sums of the inverse
 *          of number triangle A(n,k) = 1/C(n) if k <= n <= 2k, 0 otherwise,
 *          where C(n) = A000108(n).
 * https://oeis.org/A127770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A127768(2n+1) where A127768(m) is the row sum of the *
 *            inverse of the triangle T(n,k) = 1/C_n if k <= n <= 2k, 0
 *            otherwise. * Let s_n be that row sum. Then s_0 = 1 and for n >=
 *            1 * sum_{k=ceil(n/2)}^n s_k = C_n * where C_n is the Catalan
 *            number. From this we obtain * s_n = C_n + sum_{k=cei...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"
#include "../utils/seq_one.h"

seq_t a(idx_t n)
{
    /* Compute the n-th term a(n) = s_{2n+1}. */
    idx_t need = 2 * n + 1;           /* index of required row sum  */
    idx_t size = need + 1;           /* number of entries needed     */

    /* Allocate temporary arrays for s and its prefix sums P. */
    seq_t *s = seq_alloc_new_array(size);
    seq_t *P = seq_alloc_new_array(size);

    /* Catalan number C_0 = 1. */
    seq_t Cprev = seq_one();          /* C_{i-1} during the loop     */
    s[0]      = seq_one();           /* s_0 = 1                     */
    P[0]      = seq_one();           /* P_0 = 1                     */

    for (idx_t i = 1; i <= need; ++i)
    {
        /* Compute Catalan C_i from C_{i-1}:
           C_i = C_{i-1} * (4*(i-1)+2) / (i+1). */
        seq_t num = Cprev;
        num *= (4 * (i - 1) + 2);     /* numerator = Cprev * (4n+2)  */
        seq_t denom = (i - 1) + 2;    /* denominator = n+2           */
        seq_t Ci = num / denom;     /* exact division              */

        /* Indices for the summation limits. */
        idx_t mid = (i + 1) / 2;     /* ceil(i/2) */
        idx_t idx = mid - 1;         /* ceil(i/2)-1 */

        /* sumPrev = P[ceil(i/2)-1] if idx >= 0, else 0. */
        seq_t sumPrev = 0;
        if (idx <= need) sumPrev = P[idx];

        /* Compute s_i = C_i + sumPrev - P[i-1] */
        seq_t sin = Ci;
        sin += sumPrev;
        sin -= P[i - 1];

        /* Update arrays. */
        s[i] = sin;
        P[i] = P[i - 1] + sin;

        /* Prepare for next iteration. */
        Cprev = Ci;
    }

    seq_t result = s[need];

    /* Free temporary arrays. */
    seq_free_array(s);
    seq_free_array(P);

    return result;
}
