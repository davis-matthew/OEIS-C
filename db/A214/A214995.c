/*
 * A214995  Power ceiling-floor sequence of (golden ratio)^6.
 * https://oeis.org/A214995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*a(n-1) + 17*a(n-2) - a(n-3), a(0)=18, a(1)=322,
 *            a(2)=5779;
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)18;
    if (n == 1) return (seq_t)322;
    if (n == 2) return (seq_t)5779;

    seq_t a0 = (seq_t)18;
    seq_t a1 = (seq_t)322;
    seq_t a2 = (seq_t)5779;

    for (idx_t i = 3; i <= n; ++i) {
        seq_t ai = a2 * (seq_t)17 + a1 * (seq_t)17 - a0;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
