/*
 * A041035  Denominators of continued fraction convergents to sqrt(22).
 * https://oeis.org/A041035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 394*a(n-6) - a(n-12)  for n ≥ 12
 * initial values: a(0)=1, a(1)=1, a(2)=3, a(3)=13, a(4)=29, a(5)=42,
 * a(6)=365, a(7)=407, a(8)=1179, a(9)=5123, a(10)=11425, a(11)=16548
 */

#include <stddef.h>   /* for size_t */

static const seq_t init[12] = {
    (seq_t)1,   (seq_t)1,   (seq_t)3,   (seq_t)13,  (seq_t)29,   (seq_t)42,
    (seq_t)365, (seq_t)407, (seq_t)1179,(seq_t)5123,(seq_t)11425,(seq_t)16548
};

seq_t a(idx_t n)
{
    if (n < 12)
        return init[(size_t)n];

    seq_t buf[12];
    for (size_t i = 0; i < 12; ++i)
        buf[i] = init[i];

    for (idx_t i = 12; i <= n; ++i) {
        size_t idx1 = (size_t)((i - 6) % 12);
        size_t idx2 = (size_t)((i - 12) % 12);
        seq_t val = (seq_t)394 * buf[idx1] - buf[idx2];
        buf[i % 12] = val;
    }

    return buf[n % 12];
}
