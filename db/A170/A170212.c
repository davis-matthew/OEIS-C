/*
 * A170212  Number of reduced words of length n in Coxeter group on 11
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A170212 – reduced words of length n in a Coxeter group
 * with 11 generators, (S_i)^2 = I and (S_i S_j)^40 = I.
 *
 * For 0 ≤ n ≤ 39  :  a(n) = 11·10^(n-1)   (a(0)=1, a(1)=11, …).
 *
 * For n ≥ 40      :  a(n) = 9·∑_{i=1}^{39} a(n-i) – 45·a(n-40)
 *   (derived from the rational generating function)
 */
#include <stddef.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* trivial small values */
    if (n == 0) return (seq_t)1;
    if (n <= 39) return (seq_t)11 * pow10(n - 1);

    /* circular buffer for the last 40 terms (a(n-40) … a(n-1)) */
    seq_t ring[40];
    ring[0] = (seq_t)1;
    for (idx_t i = 1; i <= 39; ++i)
        ring[i] = (seq_t)11 * pow10(i - 1);

    /* initial sum of a(1)..a(39) */
    seq_t sum = (seq_t)0;
    for (idx_t i = 1; i <= 39; ++i)
        sum += ring[i];

    /* iterate from n = 40 up to the requested n */
    for (idx_t m = 40; m <= n; ++m)
    {
        idx_t prev40 = (m - 40) % 40;
        idx_t prev39 = (m - 39) % 40;
        /* a(m) = 9 * sum – 45 * a(m-40) */
        seq_t term = (sum * (seq_t)9) - (ring[prev40] * (seq_t)45);

        /* update sliding window */
        sum = sum + term - ring[prev39];
        ring[m % 40] = term;
    }

    return ring[n % 40];
}
