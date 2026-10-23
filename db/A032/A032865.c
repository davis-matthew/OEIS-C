/*
 * A032865  Numbers whose base-10 representation Sum_{i=0..m} d(i)*10^i has
 *          d(m) > d(m-1) < d(m-2) > ...
 * https://oeis.org/A032865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 7276, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A032865: numbers whose decimal digits alternate
   (d(m) > d(m-1) < d(m-2) > ...).  The code scans integers in
   increasing order and returns the n‑th number having that property. */
#include <stdbool.h>
#include <stddef.h>

static bool is_alternating(seq_t x)
{
    unsigned char dig[50];          /* enough for any seq_t (≤ 39 decimal digits) */
    size_t len = 0;
    seq_t t = x;

    if (t == 0) {
        dig[0] = 0;
        len = 1;
    } else {
        while (t > 0) {
            dig[len++] = (unsigned char)(t % 10);
            t /= 10;
        }
    }

    if (len <= 1) return true;

    for (size_t i = len; i-- > 1; ) {
        if (((len - 1 - i) & 1) == 0) {          /* even distance from MSD: must be greater */
            if (!(dig[i] > dig[i - 1]))
                return false;
        } else {                                 /* odd distance: must be less   */
            if (!(dig[i] < dig[i - 1]))
                return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n<1, return 0 as guard */

    seq_t val = 0;
    idx_t cnt = 0;
    while (1) {
        if (is_alternating(val)) {
            ++cnt;
            if (cnt == n)
                return val;
        }
        ++val;
    }
}
