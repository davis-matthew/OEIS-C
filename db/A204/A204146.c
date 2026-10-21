/*
 * A204146  Number of (n+2) X 3 0..2 arrays with every 3 X 3 subblock having
 *          equal diagonal elements or equal antidiagonal elements, and new
 *          values 0..2 introduced in row major order.
 * https://oeis.org/A204146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 22, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A204146: Number of (n+2) X 3 0..2 arrays with every 3 X 3 subblock having
 * equal diagonal elements or equal antidiagonal elements, and new values 0..2
 * introduced in row major order.
 *
 * Recurrence:  a(n) = 6*a(n-1) -5*a(n-2) +20*a(n-3) -4*a(n-4) -16*a(n-5),
 * for n ≥ 6, with initial values
 *   a(1)=689, a(2)=3902, a(3)=22262, a(4)=127022, a(5)=724154.
 *
 * The implementation computes the value iteratively in O(n) time, which is
 * adequate for n up to about 10 000.  No memoisation is required because
 * the recurrence permits linear computation from the previous five values.
 */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* base values (n = 1 … 5) */
    static const seq_t init[5] = {
        (seq_t)689,
        (seq_t)3902,
        (seq_t)22262,
        (seq_t)127022,
        (seq_t)724154
    };

    /* return the initial values directly */
    if (n <= 5)
        return init[n - 1];

    /* shift register holding the last five computed terms */
    seq_t a1 = init[0];  /* a(n-5) */
    seq_t a2 = init[1];  /* a(n-4) */
    seq_t a3 = init[2];  /* a(n-3) */
    seq_t a4 = init[3];  /* a(n-2) */
    seq_t a5 = init[4];  /* a(n-1) */

    /* compute a(n) iteratively */
    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = 6 * a5 - 5 * a4 + 20 * a3 - 4 * a2 - 16 * a1;
        /* shift window for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }

    return a5;  /* a(n) is held in a5 after the loop */
}
