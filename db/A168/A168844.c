/*
 * A168844  Number of reduced words of length n in Coxeter group on 23
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^20 = I.
 * https://oeis.org/A168844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A168844: number of reduced words of length n in a Coxeter group
 *  on 23 generators with relations (S_i)^2 = (S_i S_j)^20 = I.
 *
 *  The ordinary generating function is
 *      (t^20 + 2t^19 + … + 2t^2 + 2t + 1) /
 *      (231 t^20 – 21 t^19 – 21 t^18 – … – 21 t + 1).
 *
 *  From this we obtain the linear recurrence (valid for all n ≥ 0)
 *
 *      a(n) = b(n) + 21·∑_{k=1}^{19} a(n−k) – 231·a(n−20)
 *
 *  where b(n) = 1 for n = 0 or n = 20, 2 for 1 ≤ n ≤ 19, and 0 otherwise.
 *
 *  Initial values for 0 ≤ n < 20 are generated with the recurrence using
 *  only the already known terms.
 *
 *  The implementation keeps a circular buffer of the last 20 values
 *  and memoises all previously computed terms so that any call order
 *  is handled correctly.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /*  buffer holds the last 20 computed terms,
     *  divided into two parts:
     *      cache[i]  for 0 ≤ i < 20   (indices modulo 20)
     *  computed keeps the number of terms already known (0 = none).
     */
    static seq_t cache[20];
    static idx_t computed = 0;     /* number of terms already computed */

    /* extend computation up to n (inclusive) if necessary */
    for (idx_t i = computed; i <= n; ++i) {
        if (i < 20) { /* initial part (terms 0 … 19) */
            seq_t sum = 0;
            for (idx_t j = 0; j < i; ++j)
                sum += cache[j];

            seq_t b = (i == 0) ? 1 : 2;
            cache[i] = sum * ((seq_t)21) + b;
        } else {        /* full recurrence for i ≥ 20 */
            /* value of a(i-20) before it gets overwritten */
            seq_t a_prev20 = cache[i % 20];

            /* sum of the previous 19 terms a(i‑1)…a(i‑19) */
            seq_t sum19 = 0;
            for (idx_t k = 1; k <= 19; ++k)
                sum19 += cache[(i - k) % 20];

            /* b(i) is 1 only for i = 20, 0 otherwise */
            seq_t b = (i == 20) ? 1 : 0;

            /* compute a(i) */
            seq_t val = b + sum19 * ((seq_t)21) - a_prev20 * ((seq_t)231);
            cache[i % 20] = val;   /* overwrite the oldest value */
        }
    }

    computed = n + 1;
    return (n < 20) ? cache[n] : cache[n % 20];
}
