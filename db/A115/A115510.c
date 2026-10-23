/*
 * A115510  a(1)=1. a(n) is smallest positive integer not occurring earlier in
 *          the sequence such that a(n) and a(n-1) have at least one 1-bit in
 *          the same position when they are written in binary.
 * https://oeis.org/A115510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 1025, u32 n <= 1025, u64 n <= 1025,
 *            u128 n <= 1025, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A115510: a(1)=1, a(2)=3, a(3)=2, a(4)=6, a(5)=4, a(6)=5;
   For n ≥ 7, a(n) = n unless n is a power of two, in which case
   a(n) = n+1; if n-1 is a power of two, a(n) = n-1.  This implements
   the pattern (4,6,5) as a special 3‑cycle and the (2ᵏ,2ᵏ+1) 2‑cycles. */

#include <stdbool.h>

static inline bool is_power_of_two(idx_t x)
{
    return x != 0 && (x & (x - 1)) == 0;
}

seq_t a(idx_t n)
{
    /* handle the explicitly known small values */
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)3;
    if (n == 3)
        return (seq_t)2;
    if (n == 4)
        return (seq_t)6;
    if (n == 5)
        return (seq_t)4;
    if (n == 6)
        return (seq_t)5;

    /* n >= 7 */
    if (is_power_of_two(n))
        return (seq_t)(n + 1);
    if (is_power_of_two(n - 1))
        return (seq_t)(n - 1);
    return (seq_t)n;
}
