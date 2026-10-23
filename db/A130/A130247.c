/*
 * A130247  Inverse Lucas (A000032) numbers: index k of a Lucas number such
 *          that Lucas(k)=n; max(k|Lucas(k) < n), if there is no such index.
 * https://oeis.org/A130247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{k | Lucas(k) < n} (returns k if Lucas(k)=n)
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are defined by the build environment. */

seq_t a(idx_t n)
{
    /* Special cases for n=1 and n=2.  For n>2 the routine below
     * searches the Lucas sequence L(0)=2, L(1)=1, L(k)=L(k-1)+L(k-2).  */
    if (n == 1u) return 1u;   /* Lucas(1)=1 */
    if (n == 2u) return 0u;   /* Lucas(0)=2 */

    seq_t l_prevprev = 2u; /* L(0) */
    seq_t l_prev     = 1u; /* L(1) */
    idx_t k = 2u;          /* index of the next Lucas number to compute */

    for (;; ++k)
    {
        seq_t l_curr = l_prevprev + l_prev;  /* L(k) */

        if (l_curr == n)
            return k;            /* exact match */

        if (l_curr > n)
            return k-1u;         /* largest index with L < n */

        /* advance the window */
        l_prevprev = l_prev;
        l_prev     = l_curr;
    }
}
