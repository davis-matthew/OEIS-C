/*
 * A001389  To get the 6th term, for example, note that 5th term has three (10
 *          in ternary!) 1's, one (1) 0, etc., giving 10 1 1 0 1 2 2 1 1 2.
 * https://oeis.org/A001389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 2; a(n) = ternary look-and-say description of a(n-1):
 *            each run of c copies of digit d becomes (c written in base 3)
 *            followed by d. Digits are kept in a growable char buffer and
 *            converted to seq_t only at the end
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    size_t len = 1, cap = 16;
    unsigned char *cur = (unsigned char *)malloc(cap);
    cur[0] = 2;
    for (idx_t it = 2; it <= n; it++) {
        size_t ncap = 2 * len + 8;
        unsigned char *nxt = (unsigned char *)malloc(ncap);
        size_t nl = 0, pos = 0;
        while (pos < len) {
            unsigned char d = cur[pos];
            size_t c = 0;
            while (pos < len && cur[pos] == d) { c++; pos++; }
            unsigned char t[64];
            int tl = 0;
            while (c > 0) { t[tl++] = (unsigned char)(c % 3); c /= 3; }
            while (tl > 0) nxt[nl++] = t[--tl];
            nxt[nl++] = d;
        }
        free(cur);
        cur = nxt;
        len = nl;
    }
    seq_t r = 0;
    for (size_t i = 0; i < len; i++) {
        r = r * 10 + cur[i];
    }
    free(cur);
    return r;
}
