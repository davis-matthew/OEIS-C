/*
 * A037109  Trajectory of 3 under map n->23n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A037109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Trajectory of 3 under the map:
 *   if n is odd:  n -> 23 n + 1
 *   if n is even: n -> n / 2
 * Sequence: a(0) = 3,
 *           a(n) = (a(n‑1) odd ? 23 a(n‑1)+1 : a(n‑1)/2)   for n ≥ 1   */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)3;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        if ((cur & (seq_t)1) != (seq_t)0)   /* odd */
            cur = cur * (seq_t)23 + (seq_t)1;
        else                               /* even */
            cur = cur / (seq_t)2;
    }
    return cur;
}
