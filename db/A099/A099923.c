/*
 * A099923  Fourth powers of Lucas numbers A000032.
 * https://oeis.org/A099923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(n)^4 where Lucas(0)=2, Lucas(1)=1,
 *            Lucas(n)=Lucas(n‑1)+Lucas(n‑2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly. */
    if (n == 0) return (seq_t)16;   /* 2^4 */
    if (n == 1) return (seq_t)1;    /* 1^4 */

    /* Iteratively compute the n‑th Lucas number. */
    seq_t l_prev2 = 2; /* Lucas(0) */
    seq_t l_prev1 = 1; /* Lucas(1) */
    seq_t l_curr;

    for (idx_t i = 2; i <= n; ++i) {
        l_curr   = l_prev1 + l_prev2; /* Lucas(i) = Lucas(i‑1) + Lucas(i‑2) */
        l_prev2  = l_prev1;
        l_prev1  = l_curr;
    }

    /* l_prev1 now holds Lucas(n).  Compute (Lucas(n)^2)^2 = Lucas(n)^4. */
    seq_t sq     = l_prev1 * l_prev1;   /* Lucas(n)^2 */
    return sq * sq;                     /* Lucas(n)^4 */
}
