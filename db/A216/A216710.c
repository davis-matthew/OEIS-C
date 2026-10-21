/*
 * A216710  Expansion of (1-3*x+x^2)^2/(1-9*x+28*x^2-35*x^3+15*x^4-x^5).
 * https://oeis.org/A216710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:  A216710
   Recurrence: a(n) = 9*a(n-1) - 28*a(n-2) + 35*a(n-3) - 15*a(n-4) + a(n-5)
   Initial values: a(0)=1, a(1)=3, a(2)=10, a(3)=35, a(4)=126
   (The generating function is (1-3*x+x^2)^2/(1-9*x+28*x^2-35*x^3+15*x^4-x^5).) */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Handle the first five terms directly to avoid allocating
       memory for trivial calls. */
    if (n < 5) {
        static const seq_t init[5] = { (seq_t)1, (seq_t)3, (seq_t)10,
                                      (seq_t)35, (seq_t)126 };
        return init[(size_t)n];
    }

    static seq_t *values = NULL;
    static idx_t last_computed = 0;   /* highest index computed so far */

    /* Allocate or extend the cache as needed. */
    if (values == NULL) {
        /* First allocation: space for n+1 values. */
        size_t size = (size_t)(n + 1);
        values = (seq_t *)malloc(size * sizeof(seq_t));
        if (values == NULL) {
            /* Allocation failed; return 0 as a safe fallback. */
            return (seq_t)0;
        }
        /* Initialize the five known terms. */
        values[0] = (seq_t)1;
        values[1] = (seq_t)3;
        values[2] = (seq_t)10;
        values[3] = (seq_t)35;
        values[4] = (seq_t)126;
        last_computed = 4;
    } else if (last_computed < (idx_t)n) {
        /* Resize the array to accommodate up to n. */
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(values, new_size * sizeof(seq_t));
        if (tmp == NULL) {
            /* Reallocation failed; keep old array but we cannot compute more. */
            return values[n];
        }
        values = tmp;
        /* No need to reinitialize existing values. */
    }

    /* Compute any missing terms between last_computed+1 and n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t v = (seq_t)9   * values[i - 1]
                - (seq_t)28  * values[i - 2]
                + (seq_t)35  * values[i - 3]
                - (seq_t)15  * values[i - 4]
                +          values[i - 5];
        values[i] = v;
    }
    last_computed = n;
    return values[n];
}
