/*
 * A036415  Values of k for which there are no empty intervals when fractional
 *          part(m*phi) for m = 1, ..., k is plotted along [ 0, 1 ] subdivided
 *          into k equal regions.
 * https://oeis.org/A036415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 27, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A036415.
   For n ≤ 10 the values are given explicitly.  For n > 10 we have
   a(n) = Fib(n-3), where Fib(0)=0, Fib(1)=1.  The Fibonacci integers
   are produced via repeated addition on seq_t values. */
#include <stdint.h>

static const seq_t pre[10] = {1,2,3,4,5,6,8,10,13,16};

seq_t a(idx_t n)
{
    /* OEIS offset: a(1) is the first term. */
    if (n <= 10) {
        /* n is 1‑based; convert to 0‑based index. */
        return pre[n-1];
    }

    /* n > 10: compute Fib(n-3).  Use iterative addition. */
    idx_t k = n - 3;          /* k ≥ 8 */
    seq_t a_fib = 0;
    seq_t b_fib = 1;

    if (k == 0) return a_fib;      /* not used for n>10 */
    if (k == 1) return b_fib;      /* not used for n>10 */

    for (idx_t i = 2; i <= k; ++i) {
        seq_t temp = a_fib + b_fib;
        a_fib = b_fib;
        b_fib = temp;
    }
    return b_fib;
}
