/*
 * A039198  Numbers whose base-11 representation has the same number of 5's
 *          and 9's.
 * https://oeis.org/A039198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th non‑negative integer whose base‑11
 *            representation contains an equal number of digits 5 and 9. The
 *            first term is a(1)=0
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool has_equal_59(seq_t num)
{
    int c5 = 0, c9 = 0;
    if (num == (seq_t)0)
        return true;                     /* zero has 0 fives and 0 nines */

    while (num > (seq_t)0) {
        seq_t d = num % (seq_t)11;
        if (d == (seq_t)5)   c5++;
        else if (d == (seq_t)9) c9++;
        num /= (seq_t)11;
    }
    return c5 == c9;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t  num   = (seq_t)0;

    for (;;) {
        if (has_equal_59(num)) {
            ++found;
            if (found == n)
                return num;
        }
        ++num;
    }
}
