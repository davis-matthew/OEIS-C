/*
 * A024358  Sum of the sizes of binary subtrees of the perfect binary tree of
 *          height n.
 * https://oeis.org/A024358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = B'_n(1) where B_{n+1}(x) = 1 + x*B_n(x)^2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: b_0 = 1, a_0 = 0; for n≥0,
   b_{n+1} = b_n^2 + 1,
   a_{n+1} = b_n^2 + 2*b_n*a_n;
   a(n) = a_n. */
#include <stddef.h>

seq_t a(idx_t n)
{
    idx_t i;
    seq_t a_val = 0; /* a_0 */
    seq_t b_val = 1; /* b_0 */

    /* Early return for n = 0 */
    if (n == 0)
        return a_val;

    for (i = 0; i < n; ++i) {
        /* Compute next values using previous ones */
        seq_t b_next = b_val * b_val + 1;          /* b_{i+1} */
        seq_t a_next = b_val * b_val + 2 * b_val * a_val; /* a_{i+1} */
        a_val = a_next;
        b_val = b_next;
    }

    return a_val;
}
