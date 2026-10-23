/*
 * A037117  Trajectory of 3 under map n->39n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A037117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 22, u64 n <= 53, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=3; a(n) = (a(n-1) odd? 39*a(n-1)+1 : a(n-1)/2) */

#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t cur = 3;
    if (n == 0) return cur;
    for (idx_t i = 1; i <= n; ++i) {
        if (cur & 1)            /* odd */
            cur = cur * (seq_t)39 + (seq_t)1;
        else                    /* even */
            cur >>= 1;
    }
    return cur;
}
