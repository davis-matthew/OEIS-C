/*
 * A211435  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...,0,...,n} and w+4x+5y=0.
 * https://oeis.org/A211435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A211435
   Formula: a(n) = the number of integer triples (w,x,y) with each component in
                [-n, n] satisfying w + 4x + 5y = 0.
   Computation: For each y ∈ [-n,n] count integers x with |4x + 5y| ≤ n;
   w is then forced.  The sum over all y gives a(n).  O(n) time.
*/

#include <stdint.h>
#include <stdbool.h>

/* Helper functions for integer ceil/floor division (b > 0).   */
static inline int64_t ceil_div(int64_t a, int64_t b) {
    if (a >= 0) return (a + b - 1) / b;
    else return a / b;   /* truncates toward zero, which equals ceil for a<0 */
}
static inline int64_t floor_div(int64_t a, int64_t b) {
    if (a >= 0) return a / b;
    else return (a - b + 1) / b;   /* truncates toward zero → floor for a<0 */
}

seq_t a(idx_t n) {
    int64_t N = (int64_t)n;          /* signed copy for arithmetic */
    seq_t total = 0;                 /* accumulate result in seq_t */

    for (int64_t y = -N; y <= N; ++y) {
        int64_t a_min = -N - 5 * y;  /* lower bound for 4x: -n - 5y */
        int64_t a_max =  N - 5 * y;  /* upper bound for 4x:  n - 5y */

        int64_t x_min = ceil_div(a_min, 4);
        if (x_min < -N) x_min = -N;
        int64_t x_max = floor_div(a_max, 4);
        if (x_max >  N) x_max =  N;

        if (x_max >= x_min) {
            /* x_max - x_min + 1 fits in int64_t for all n considered */
            total += (seq_t)( (x_max - x_min + 1) );
        }
    }

    return total;
}
