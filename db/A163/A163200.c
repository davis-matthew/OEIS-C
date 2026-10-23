/*
 * A163200  Sum of the cubes of the first n odd-indexed Fibonacci numbers.
 * https://oeis.org/A163200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=9, a(3)=134;
 *            a(n)=21*a(n-1)-56*a(n-2)+21*a(n-3)-a(n-4) for n>=4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 9;
    if (n == 3) return 134;

    /* For small seq_t (≤ 64‑bit) use a 128‑bit intermediate to avoid overflow. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 a0 = 0, a1 = 1, a2 = 9, a3 = 134;
        for (idx_t i = 4; i <= n; ++i) {
            unsigned __int128 next = 21 * a3 - 56 * a2 + 21 * a1 - a0;
            a0 = a1; a1 = a2; a2 = a3; a3 = next;
        }
        return (seq_t)a3;
    }

    /* For arbitrary‑precision or 128‑bit seq_t, use normal integer arithmetic. */
    seq_t a0 = 0, a1 = 1, a2 = 9, a3 = 134;
    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = a3 * (seq_t)21;
        next = next - a2 * (seq_t)56;
        next = next + a1 * (seq_t)21;
        next = next - a0;
        a0 = a1; a1 = a2; a2 = a3; a3 = next;
    }
    return a3;
}
