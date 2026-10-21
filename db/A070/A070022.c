/*
 * A070022  1/n has period 2 in base 10.
 * https://oeis.org/A070022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 11*A070023(n). Numbers of the form a*b*c where a is in
 *            {11,33,99}, b is a power of 2, c is a power of 5
 * Range:     u8 n <= 11, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, so this case is unused */

    /* Generate the first "cap" humble numbers (powers of 2 and 5). */
    size_t cap = (size_t)n + 5;     /* a few extra to be safe */
    seq_t *hum = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!hum) return 0;            /* allocation failure – unlikely */

    hum[0] = 1;
    size_t len = 1;
    size_t i2 = 0, i5 = 0;

    while (len < cap) {
        seq_t v2 = hum[i2] * 2;
        seq_t v5 = hum[i5] * 5;
        seq_t next = v2 < v5 ? v2 : v5;
        hum[len++] = next;
        if (next == v2) ++i2;
        if (next == v5) ++i5;
    }

    /* Merge the three sequences 11*hum, 33*hum, 99*hum. */
    size_t p11 = 0, p33 = 0, p99 = 0;
    seq_t cur11 = hum[p11] * 11;
    seq_t cur33 = hum[p33] * 33;
    seq_t cur99 = hum[p99] * 99;
    seq_t result = 0;

    for (uint64_t step = 1; step <= n; ++step) {
        if (cur11 <= cur33 && cur11 <= cur99) {
            result = cur11;
            ++p11;
            if (p11 < cap) cur11 = hum[p11] * 11;
            else cur11 = (seq_t)-1;   /* sentinel for "infinite" */
        } else if (cur33 <= cur11 && cur33 <= cur99) {
            result = cur33;
            ++p33;
            if (p33 < cap) cur33 = hum[p33] * 33;
            else cur33 = (seq_t)-1;
        } else {
            result = cur99;
            ++p99;
            if (p99 < cap) cur99 = hum[p99] * 99;
            else cur99 = (seq_t)-1;
        }
    }

    free(hum);
    return result;
}
