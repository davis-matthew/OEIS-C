/*
 * A098531  Sum of fifth powers of first n Fibonacci numbers.
 * https://oeis.org/A098531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=1..n} F_k^5 (F_0 = 0, F_1 = 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   pow5_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow5_local.h"

seq_t a(idx_t n)
{
    /* Static cache of Fibonacci numbers and accumulated sums */
    static seq_t *fib = NULL;      /* Fibonacci numbers F_i, i >= 0 */
    static seq_t *sum = NULL;      /* a(i) = sum_{k=1..i} F_k^5, sum[0] = 0 */
    static size_t capacity = 0;    /* allocated array size (number of elements) */
    static idx_t  max_n = 0;       /* highest n computed so far */

    if (fib == NULL) {           /* first-time initialization */
        capacity = 2;            /* at least indices 0 and 1 needed */
        fib  = (seq_t *)malloc(capacity * sizeof(seq_t));
        sum  = (seq_t *)malloc(capacity * sizeof(seq_t));
        fib[0] = 0;
        sum[0] = 0;
    }

    /* Expand cache to hold up to n */
    if (n > max_n) {
        size_t needed = (size_t)n + 1;
        if (needed > capacity) {
            /* Grow by doubling until we have enough space */
            while (capacity < needed) capacity <<= 1;
            fib = (seq_t *)realloc(fib, capacity * sizeof(seq_t));
            sum = (seq_t *)realloc(sum, capacity * sizeof(seq_t));
        }
        /* Compute Fibonacci numbers and sums from max_n+1 up to n */
        for (size_t i = (size_t)max_n + 1; i <= (size_t)n; ++i) {
            if (i == 1) {
                fib[i] = 1;
            } else {
                fib[i] = fib[i - 1] + fib[i - 2];
            }
            seq_t p5 = pow5_local(fib[i]);     /* fib[i]^5 */
            sum[i] = sum[i - 1] + p5;
        }
        max_n = n;
    }

    return sum[(size_t)n];
}
