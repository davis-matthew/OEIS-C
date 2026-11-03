/*
 * A193582  Persistence of n for sort-and-subtract (A193581).
 * https://oeis.org/A193582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of sort‑and‑subtract steps to reach 0 * (at each
 *            step n is replaced by n - (ascending order of its digits))
 * Range:     u8 n <= 1418, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9838 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t curr = (seq_t)n;
    seq_t steps = 0;
    char buf[128];
    size_t len;
    unsigned int cnt[10];
    char asc[128];

    while (curr != 0)
    {
        /* Convert current number to decimal string to count digits */
        seq_to_str(curr, buf, &len);
        for (int i = 0; i < 10; ++i) cnt[i] = 0;
        for (size_t i = 0; i < len; ++i)
            ++cnt[(unsigned int)(buf[i] - '0')];

        /* Build ascending string representing the smallest permutation */
        size_t idx = 0;
        for (int d = 0; d < 10; ++d)
            for (unsigned int k = 0; k < cnt[d]; ++k)
                asc[idx++] = (char)('0' + d);
        asc[idx] = '\0';

        seq_t asc_num = seq_from_str(asc);
        curr = curr - asc_num;
        ++steps;
    }
    return steps;
}
