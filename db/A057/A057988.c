/*
 * A057988  Positions of 2 in A057985.
 * https://oeis.org/A057988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th 2 in A057985, the fixed point of
 *            0->01, 1->12, 2->0 (computed by iterating the substitution)
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 10000 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static unsigned char *w = NULL;
    static size_t len = 0;
    static idx_t *pos = NULL;
    static size_t npos = 0;

    if (w == NULL) {
        w = (unsigned char *)malloc(1);
        w[0] = 0;
        len = 1;
    }

    while (npos < (size_t)n) {
        size_t newlen = 0, i, j = 0;
        for (i = 0; i < len; i++)
            newlen += (w[i] == 2) ? 1 : 2;
        unsigned char *nw = (unsigned char *)malloc(newlen);
        for (i = 0; i < len; i++) {
            unsigned char v = w[i];
            if (v == 0) { nw[j++] = 0; nw[j++] = 1; }
            else if (v == 1) { nw[j++] = 1; nw[j++] = 2; }
            else { nw[j++] = 0; }
        }
        free(w);
        w = nw;
        len = newlen;

        size_t c = 0;
        for (i = 0; i < len; i++)
            if (w[i] == 2) c++;
        free(pos);
        pos = (idx_t *)malloc((c ? c : 1) * sizeof(idx_t));
        size_t k = 0;
        for (i = 0; i < len; i++)
            if (w[i] == 2) pos[k++] = (idx_t)(i + 1);
        npos = c;
    }

    return (seq_t)pos[n - 1];
}
