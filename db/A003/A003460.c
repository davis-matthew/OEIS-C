/*
 * A003460  Octal formula for dragon curve of order n.
 * https://oeis.org/A003460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   binary string b(1) = "1", b(n) = b(n-1) . "1" . b'(n-1), where
 *            b'(n-1) is b(n-1) with its middle bit flipped; a(n) = the octal
 *            digits of b(n), read as a decimal number
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    size_t L = 1, total, i, k;
    unsigned char *s;
    seq_t result = 0;
    unsigned lead, d;

    if (n < 1) return 0;
    total = (((size_t)1) << n) - 1;
    s = (unsigned char *)malloc(total);
    if (!s) return 0;
    s[0] = 1;
    for (k = 1; k < (size_t)n; k++) {
        size_t mid = (L - 1) / 2;
        s[L] = 1;
        memcpy(s + L + 1, s, L);
        s[L + 1 + mid] ^= 1;
        L = 2 * L + 1;
    }

    /* group bits into octal digits from the right; the leading group may be short */
    lead = (unsigned)(L % 3);
    i = 0;
    if (lead) {
        d = 0;
        for (k = 0; k < lead; k++) d = d * 2 + s[i++];
        result = (seq_t)d;
    }
    while (i < L) {
        d = (unsigned)(s[i] * 4 + s[i + 1] * 2 + s[i + 2]);
        i += 3;
        result = result * 10 + d;
    }
    free(s);
    return result;
}
