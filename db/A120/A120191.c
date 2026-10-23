/*
 * A120191  a(1)=6; a(n)=floor((47+sum(a(1) to a(n-1)))/7).
 * https://oeis.org/A120191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 6; for n > 1: a(n) = floor((47 + Σ_{i=1}^{n-1} a(i)) / 7)
 *            with Σ the cumulative sum of all previous terms. This
 *            implementation memoises the sequence up to the maximum index
 *            requested to handle arbitrary call order efficiently
 * Range:     u8 n <= 15, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* Undefined for n = 0; return 0 for safety */

    static seq_t *a_vals  = NULL;   /* a(1) .. a(max_n)  */
    static seq_t *s_vals  = NULL;   /* S(n) = Σ_{i=1}^{n} a(i)  */
    static idx_t max_n  = 0;        /* highest index computed */
    static size_t cap   = 0;        /* current allocation capacity */

    /* Allocate or grow storage if needed */
    if (n > max_n) {
        size_t needed = (size_t)(n + 1);      /* +1 to use 1‑based indices */
        if (cap == 0) {
            cap = 16;
        }
        while (cap < needed) {
            cap <<= 1;                       /* double until sufficient */
        }

        a_vals = (seq_t *)realloc(a_vals, cap * sizeof(seq_t));
        s_vals = (seq_t *)realloc(s_vals, cap * sizeof(seq_t));

        /* Initialise the first element of the cumulative sum array */
        if (max_n == 0) {
            s_vals[0] = 0;
            a_vals[0] = 0;
        }

        /* Compute terms from max_n + 1 up to n */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            size_t idx = (size_t)i;
            seq_t prev_sum = s_vals[idx - 1];
            seq_t val = ((seq_t)47 + prev_sum) / 7;   /* floor division */
            a_vals[idx] = val;
            s_vals[idx] = prev_sum + val;
        }
        max_n = n;
    }

    return a_vals[n];
}
