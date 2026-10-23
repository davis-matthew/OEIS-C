/*
 * A102429  Row sums of A102427.
 * https://oeis.org/A102429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3) - a(n-4) + a(n-5) + b(n), *
 *            where a(0)=1, a(1)=2, a(2)=6, a(3)=11, a(4)=23, a(5)=44, * and
 *            b(n) = 1 for n=0 or 2, * = -1 for n=3, * = 0 otherwise. * The
 *            generating function is (1+x^2-x^3)/((1-x)*(1-x-2x^2+x^4))
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;     /* memoization array */
static idx_t memo_size = 0;    /* number of elements stored (0 .. memo_size-1) */

seq_t a(idx_t n) {
    /* Extend memoization array to compute terms up to n */
    if (n >= memo_size) {
        seq_t *newm = (seq_t *)realloc(memo, (n + 1) * sizeof(seq_t));
        if (!newm) {
            /* Allocation failed; abort (no I/O allowed) */
            return (seq_t)0;
        }
        memo = newm;
        /* Compute missing terms */
        for (idx_t i = memo_size; i <= n; ++i) {
            seq_t term1 = 0;
            /* 2*a(i-1) */
            if (i >= 1) term1 = memo[i - 1] + memo[i - 1];
            /* + a(i-2) */
            if (i >= 2) term1 += memo[i - 2];
            /* + a(i-5) */
            if (i >= 5) term1 += memo[i - 5];

            seq_t term2 = 0;
            /* 2*a(i-3) */
            if (i >= 3) term2 = memo[i - 3] + memo[i - 3];
            /* + a(i-4) */
            if (i >= 4) term2 += memo[i - 4];

            seq_t res = term1 - term2;          /* temporary result */

            /* add b(n) */
            if (i == 0) {
                res += (seq_t)1;
            } else if (i == 2) {
                res += (seq_t)1;
            } else if (i == 3) {
                res -= (seq_t)1;  /* b(3) = -1 */
            }
            memo[i] = res;
        }
        memo_size = n + 1;
    }
    return memo[n];
}
