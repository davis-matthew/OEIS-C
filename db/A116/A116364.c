/*
 * A116364  Row squared sums of Catalan triangle A033184.
 * https://oeis.org/A116364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (C(2*n-k+1,n-k)*(k+1)/(2*n-k+1))^2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 35,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 0. */
    if (n == 0)
        return 1;

    /* t starts with k = 0: t = Catalan(n). */
    seq_t t = catalan(n);
    seq_t sum = t * t;

    /* Iterate for k = 1 .. n, updating t by the recurrence:
       t_k = t_{k-1} * (n-k+1)*(k+1) / (k*(2*n-k+1))           */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t num = (seq_t)(n - k + 1) * (k + 1);
        seq_t den = (seq_t)k * (2 * n - k + 1);
        t = t * num / den;
        sum += t * t;
    }

    return sum;
}
