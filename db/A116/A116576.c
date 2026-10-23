/*
 * A116576  Number of distinct squares D(n) in the n-th iterate of the
 *          tribonacci morphism (a -> ab, b -> ac, c -> a) on the letter a.
 * https://oeis.org/A116576
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, uned
 * Formula:   D(n) for n ≥ 6 is D(n) = sum_{m=0}^{n-3} d(m) + d(n-5) + d(n-6)
 *            + (n-1) where d(0) = 0 and for m ≥ 1 d(m) = ((T(m+1) + T(m-1) –
 *            1) / 2) – 1 with Tribonacci numbers T defined by T(0)=1, T(1)=2,
 *            T(2)=4, T(k)=T(k-1)+T(k-2)+T(k-3) for k≥3
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* offset: first term is at n = 4 */
    if (n == 4) return (seq_t)2;
    if (n == 5) return (seq_t)7;
    if (n < 6)  return (seq_t)0;   /* Undefined for n < 4, safe fallback */

    /* Allocate arrays for Tribonacci numbers and the d values.  */
    /* We need T[0 … n-2] and d[0 … n-3].  */
    idx_t t_size = n - 1;          /* n-2 + 1 elements */
    idx_t d_size = n - 2;          /* n-3 + 1 elements */
    seq_t *T = (seq_t *)calloc(t_size, sizeof(seq_t));
    seq_t *d = (seq_t *)calloc(d_size, sizeof(seq_t));
    if (!T || !d) return (seq_t)0; /* allocation failure – unlikely */

    /* Tribonacci initialization */
    T[0] = (seq_t)1;
    if (t_size > 1) T[1] = (seq_t)2;
    if (t_size > 2) T[2] = (seq_t)4;
    for (idx_t i = 3; i < t_size; ++i)
        T[i] = T[i-1] + T[i-2] + T[i-3];

    /* Compute d[0 .. n-3] */
    d[0] = (seq_t)0;
    for (idx_t i = 1; i < d_size; ++i) {
        seq_t val = T[i+1] + T[i-1];
        val -= (seq_t)1;
        val = val / (seq_t)2;
        val -= (seq_t)1;
        d[i] = val;
    }

    /* Sum of d[0 .. n-3] */
    seq_t sum = d[0];
    for (idx_t i = 1; i < d_size; ++i)
        sum += d[i];

    /* Final formula */
    seq_t result = sum;
    result += d[n-5];
    result += d[n-6];
    result += (seq_t)(n - 1);

    /* Clean up */
    free(T);
    free(d);
    return result;
}
