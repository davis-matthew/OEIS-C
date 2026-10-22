/*
 * A005678  A squarefree ternary sequence.
 * https://oeis.org/A005678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th letter of the fixed point starting with 1 of
 *            the morphism 1 -> 1213, 2 -> 2123, 3 -> 2313 (after 1) or 1323
 *            (after 2); letter j expands to letters 4j-3..4j
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    idx_t cap = n + 8, filled = 1, j = 0;
    unsigned char *s = (unsigned char *)malloc((size_t)cap);
    unsigned char r;
    s[0] = 1;
    while (filled < n) {
        unsigned char c = s[j], p = (j > 0) ? s[j - 1] : 0;
        unsigned char w[4];
        int k;
        if (c == 1) { w[0] = 1; w[1] = 2; w[2] = 1; w[3] = 3; }
        else if (c == 2) { w[0] = 2; w[1] = 1; w[2] = 2; w[3] = 3; }
        else if (p == 1) { w[0] = 2; w[1] = 3; w[2] = 1; w[3] = 3; }
        else { w[0] = 1; w[1] = 3; w[2] = 2; w[3] = 3; }
        for (k = 0; k < 4; k++) {
            idx_t pos = 4 * j + (idx_t)k;
            if (pos < cap) s[pos] = w[k];
        }
        if (4 * j + 4 > filled) filled = 4 * j + 4;
        j++;
    }
    r = s[n - 1];
    free(s);
    return (seq_t)r;
}
