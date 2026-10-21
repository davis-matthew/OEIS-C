/*
 * A217052  a(n) = 3*a(n-1) + 24*a(n-2) + a(n-3), with a(0)=a(1)=1, and
 *          a(2)=19.
 * https://oeis.org/A217052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 24*a(n-2) + a(n-3), a(0)=1, a(1)=1, a(2)=19
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle the trivial first three terms */
    if (n == (idx_t)0 || n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)19;

    /* Sliding‑window recurrence */
    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)1;   /* a(n-2) */
    seq_t a2 = (seq_t)19;  /* a(n-1) */
    seq_t an;

    for (idx_t i = (idx_t)3; i <= n; ++i) {
        an = (seq_t)3 * a2 + (seq_t)24 * a1 + a0; /* a(i) */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
