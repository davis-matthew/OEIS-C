/*
 * A063211  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          43 ).
 * https://oeis.org/A063211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 3, a(2) = 10, a(3) = 18, a(4) = 24; For n ≥ 5: a(n) =
 *            a(n‑1) + a(n‑2) – a(n‑3) (recurrence of order 3)
 * Range:     u8 n <= 36, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. */
    if (n == 0) return (seq_t)0;          /* undefined, return 0 to avoid surprises */

    /* Pre‑defined initial values.  Index 0 is unused. */
    static const seq_t init[5] = { 0, 3, 10, 18, 24 };

    if (n < 5) return init[n];            /* a(1) … a(4) are known directly */

    /* Compute the recurrence iteratively.  No memoisation is required for
       the problem size (n ≤ ~10 000).  The loop runs in O(n) time and uses
       only a few variables of type seq_t.  */

    seq_t a_nm3 = init[2];   /* a(n‑3) : start with a(2) when n == 5 */
    seq_t a_nm2 = init[3];   /* a(n‑2) : a(3) */
    seq_t a_nm1 = init[4];   /* a(n‑1) : a(4) */
    seq_t a_n   = 0;         /* current value */

    for (idx_t i = 5; i <= n; ++i) {
        a_n = a_nm1 + a_nm2 - a_nm3;  /* recurrence: a(i) = a(i-1)+a(i-2)-a(i-3) */
        a_nm3 = a_nm2;                /* shift the window */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
