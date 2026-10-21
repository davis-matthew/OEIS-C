/*
 * A235698  a(n+1) = a(n) + (smallest digit of a(n)) + 1, a(0)=0.
 * https://oeis.org/A235698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0,
   a(n+1) = a(n) + smallest digit of a(n) + 1
   (OEIS A235698) */
#include <stddef.h>
#include <stdint.h>

static seq_t smallest_digit(seq_t v)
{
    if (v == 0)
        return 0;
    seq_t m = 9;
    while (v) {
        seq_t d = v % 10;
        if (d < m)
            m = d;
        v /= 10;
    }
    return m;
}

seq_t a(idx_t n)
{
    seq_t cur = 0;          /* a(0) */
    if (n == 0)
        return cur;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t sd = smallest_digit(cur);
        cur = cur + sd + 1;
    }
    return cur;
}
