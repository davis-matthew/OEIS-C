/*
 * A151955  a(1) = 102; thereafter a(n) = (a(n-1) with digits sorted into
 *          descending order) - (a(n-1) with digits sorted into ascending
 *          order) (see the Kaprekar map, A151949).
 * https://oeis.org/A151955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(1) = 102; thereafter a(n) = (a(n-1) with digits sorted into
 *            descending order) - (a(n-1) with digits sorted into ascending
 *            order) (Kaprekar map, A151949)
 * Range:     u8 n <= 2, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Helper: compute the Kaprekar map step for a given value */
static seq_t kaprekar_next(seq_t v)
{
    /* Count decimal digits 0-9 */
    unsigned char cnt[10] = {0};
    seq_t tmp = v;
    while (tmp > 0) {
        unsigned d = (unsigned)(tmp % 10);
        cnt[d]++;
        tmp /= 10;
    }

    /* Build ascending number */
    seq_t asc = 0;
    for (int d = 0; d <= 9; ++d) {
        for (unsigned i = 0; i < cnt[d]; ++i) {
            asc = asc * 10 + d;
        }
    }

    /* Build descending number */
    seq_t desc = 0;
    for (int d = 9; d >= 0; --d) {
        for (unsigned i = 0; i < cnt[d]; ++i) {
            desc = desc * 10 + d;
        }
    }

    return desc - asc;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* out of bounds */
    if (n == 1) return (seq_t)102;           /* first term */

    seq_t cur = 102;                         /* a(1) */
    /* Enumerate until n or until the fixed point 495 is reached */
    for (idx_t i = 2; i <= n && cur != (seq_t)495; ++i) {
        cur = kaprekar_next(cur);
    }
    return cur;
}
