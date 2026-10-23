/*
 * A170020  Number of reduced words of length n in Coxeter group on 11
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence derived from the rational generating function  
 *  given in OEIS for A170020:
 *
 *      G(t) = (1 + 2t + 2t² + … + 2t³⁵ + t³⁶) /
 *             (1 – 9t – 9t² – … – 9t³⁵ + 45t³⁶)
 *
 *  The denominator leads to the linear recurrence  
 *      a(0) = 1
 *      a(n) = 11·10^(n-1)          for 1 ≤ n ≤ 35
 *      a(n) = 9·(a(n-1)+…+a(n-35)) – 45·a(n-36)   for n ≥ 36
 *
 *  The function below implements this recurrence efficiently
 *  using only integer arithmetic on the generic type seq_t.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Pre‑compute the first 36 terms (a[0] … a[35]) only once */
    static seq_t init[36];
    static int ready = 0;

    if (!ready) {
        init[0] = (seq_t)1;
        init[1] = (seq_t)11;
        seq_t v = (seq_t)11;
        for (idx_t i = 2; i < 36; ++i) {
            v *= (seq_t)10;
            init[i] = v;
        }
        ready = 1;
    }

    if (n < 36) {
        return init[(int)n];
    }

    /* Rolling array holding the last 36 computed terms */
    seq_t arr[36];
    for (int i = 0; i < 36; ++i) {
        arr[i] = init[i];
    }

    /* Sum of the 35 most recent terms (from a[1] to a[35]) */
    seq_t sum35 = (seq_t)0;
    for (int i = 1; i < 36; ++i) {
        sum35 += arr[i];
    }

    /* Iterate from n = 36 up to the desired index */
    for (idx_t cur = 36; cur <= n; ++cur) {
        /* a[cur-36] needed in the recurrence */
        seq_t prev36 = arr[(int)((cur - 36) % 36)];
        /* Next value according to the recurrence */
        seq_t next = (seq_t)9 * sum35 - (seq_t)45 * prev36;

        /* Remove a[cur-35] from the sliding window sum */
        seq_t remove_val = arr[(int)((cur - 35) % 36)];
        sum35 += next;
        sum35 -= remove_val;

        /* Store the new value */
        arr[(int)(cur % 36)] = next;
    }

    return arr[(int)(n % 36)];
}
