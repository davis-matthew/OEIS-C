/*
 * A041741  Denominators of continued fraction convergents to sqrt(390).
 * https://oeis.org/A041741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 158*a(n-4) - a(n-8) for n > 7 Initial values: a(0)=1,
 *            a(1)=1, a(2)=3, a(3)=4, a(4)=155, a(5)=159, a(6)=473, a(7)=632
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init_vals[8] = {
    (seq_t)1, (seq_t)1, (seq_t)3, (seq_t)4,
    (seq_t)155, (seq_t)159, (seq_t)473, (seq_t)632
};

seq_t a(idx_t n)
{
    if (n < 8U) {
        return init_vals[(size_t)n];
    }

    /* Buffer holding the last 8 terms of the sequence. */
    seq_t buf[8];
    for (size_t i = 0; i < 8; ++i) {
        buf[i] = init_vals[i];
    }

    /* Compute terms from 8 up to n using the recurrence. */
    for (idx_t i = 8; i <= n; ++i) {
        seq_t next = (seq_t)158 * buf[(i - 4) & 7] - buf[(i - 8) & 7];
        buf[i & 7] = next;
    }

    return buf[n & 7];
}
