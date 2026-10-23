/*
 * A102121  Iccanobirt numbers (11 of 15): a(n) = R(a(n-1) + R(a(n-2)) +
 *          R(a(n-3))), where R is the digit reversal function A004086.
 * https://oeis.org/A102121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = rev(a(n-1) + rev(a(n-2)) + rev(a(n-3))) with a(0)=0,
 *            a(1)=0, a(2)=1
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   rev, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/rev.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n) {
    static seq_t *a_arr = NULL;   /* memoization array */
    static idx_t size  = 0;       /* next unused index */
    static idx_t cap   = 0;       /* allocated capacity */

    if (n >= size) {
        /* grow array to at least index n */
        idx_t new_cap = (cap > 0) ? cap : 1;
        while (new_cap <= n) new_cap <<= 1;      /* power‑of‑two growth */
        a_arr = seq_realloc(a_arr, new_cap);
        cap   = new_cap;

        /* compute all missing terms */
        for (idx_t i = size; i <= n; ++i) {
            if (i == 0 || i == 1) {
                a_arr[i] = 0;
            } else if (i == 2) {
                a_arr[i] = 1;
            } else {
                seq_t r2 = rev(a_arr[i-2]);        /* rev(a(i-2)) */
                seq_t r3 = rev(a_arr[i-3]);        /* rev(a(i-3)) */
                seq_t sum = a_arr[i-1] + r2 + r3;  /* a(i-1)+r2+r3 */
                a_arr[i] = rev(sum);               /* rev of the sum */
            }
        }
        size = n + 1;   /* indices 0..n are now valid */
    }
    return a_arr[n];
}
