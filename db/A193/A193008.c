/*
 * A193008  Constant term of the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A193008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + a(n-3) + 2*a(n-4) - a(n-5) for n ≥
 *            5, with initial values a(0)=1, a(1)=2, a(2)=10, a(3)=31, a(4)=78
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the initial five terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)10;
    if (n == 3) return (seq_t)31;
    if (n == 4) return (seq_t)78;

    /* Sliding window for the last five computed values */
    seq_t prev[5] = { (seq_t)1, (seq_t)2, (seq_t)10, (seq_t)31, (seq_t)78 };

    /* Compute successively up to index n */
    for (idx_t i = 5; i <= n; ++i) {
        seq_t next =
            ((seq_t)4 * prev[4])   /* 4*a(n-1) */
          - ((seq_t)5 * prev[3])   /* -5*a(n-2) */
          +  prev[2]               /* +a(n-3)   */
          + ((seq_t)2 * prev[1])   /* +2*a(n-4) */
          -  prev[0];              /* -a(n-5)   */

        /* Shift window */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = next;
    }

    return prev[4];
}
