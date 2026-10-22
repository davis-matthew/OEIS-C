/*
 * A001154  Describe the previous term! (method A - initial term is 9).
 * https://oeis.org/A001154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(1) = 9; a(n) = the "look and say" description of a(n-1), read
 *            left to right: for each run of equal digits, write the run
 *            length and then the digit
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    size_t cap = 16, len = 1;
    unsigned char *cur = (unsigned char *)malloc(cap);
    cur[0] = 9;
    for (idx_t step = 1; step < n; step++) {
        size_t ncap = 2 * len + 40;
        unsigned char *nxt = (unsigned char *)malloc(ncap);
        size_t pos = 0, i = 0;
        while (i < len) {
            unsigned char d = cur[i];
            size_t run = 1;
            while (i + run < len && cur[i + run] == d) run++;
            unsigned char buf[24];
            int bl = 0;
            size_t t = run;
            while (t > 0) { buf[bl++] = (unsigned char)(t % 10); t /= 10; }
            while (bl > 0) nxt[pos++] = buf[--bl];
            nxt[pos++] = d;
            i += run;
        }
        free(cur);
        cur = nxt;
        len = pos;
        cap = ncap;
    }
    seq_t r = 0;
    for (size_t i = 0; i < len; i++) r = r * 10 + (seq_t)cur[i];
    free(cur);
    (void)cap;
    return r;
}
