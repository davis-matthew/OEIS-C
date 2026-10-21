/*
 * A095605  a(n) = 107 written in base 11 - n.
 * https://oeis.org/A095605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal value of the base-(11‑n) representation of 107
 *            (decimal)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* 0 ≤ n ≤ 10 in OEIS.  For n > 10 the sequence is not defined. */
    if (n > 10)
        return 0;

    idx_t base = 11 - n;
    if (base == 1) {
        /* 107 in base‑1 is 107 ones; produce a decimal string of 107 '1's. */
        static char s[108];
        for (idx_t i = 0; i < 107; ++i)
            s[i] = '1';
        s[107] = '\0';
        return seq_from_str(s);
    }

    /* Convert 107 (decimal) to the given base and collect its digits. */
    const seq_t value = 107;
    int digits[32] = {0};
    int sz = 0;
    seq_t tmp = value;
    while (tmp > 0) {
        seq_t rem = tmp % base;
        digits[sz++] = (int)rem;      /* rem is 0…9 for bases 2–11 */
        tmp = tmp / base;
    }
    if (sz == 0)        /* value was 0 */
        digits[sz++] = 0;

    /* Translate the digit array to a decimal integer. */
    seq_t res = 0;
    for (int i = sz - 1; i >= 0; --i)
        res = res * 10 + digits[i];

    return res;
}
