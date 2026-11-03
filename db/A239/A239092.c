/*
 * A239092  Prefix overlap of dictionary consisting of decimal expansions of 0
 *          through n.
 * https://oeis.org/A239092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{i=0}^{n-1} LP(i, i+1)
 * Range:     u8 n <= 115, u16 n <= 115, u32 n <= 115, u64 n <= 115,
 *            u128 n <= 115, big any n
 * Validated: partial, pass, 115 terms through n = 115 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where LP(i, i+1) is the length of the longest common
    prefix of the decimal expansions of i and i+1.
    The dictionary is ordered by increasing numeric value. */

#include <stdint.h>

static unsigned _prefix_overlap(idx_t a)
{
    /* Count trailing 9's in a */
    unsigned trailing9 = 0;
    idx_t tmp = a;
    while (tmp % 10 == 9) {
        ++trailing9;
        tmp /= 10;
    }

    /* Count total digits of a */
    unsigned digits = 0;
    tmp = a;
    do {
        ++digits;
        tmp /= 10;
    } while (tmp);

    /* If a consists only of 9's, the next number has more digits
       and shares no common prefix. Otherwise the overlap equals
       number of unchanged left‑hand digits. */
    return (trailing9 == digits) ? 0 : digits - trailing9 - 1;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 0; i < n; ++i) {
        sum += (seq_t)_prefix_overlap(i);
    }
    return sum;
}
