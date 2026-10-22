/*
 * A001140  Describe the previous term! (method A - initial term is 4).
 * https://oeis.org/A001140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(1) = 4; a(n) = look-and-say description of a(n-1) ("count"
 *            then "digit"), computed on a digit string
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n) {
    size_t len = 1, cap = 16;
    unsigned char *d = (unsigned char *)malloc(cap);
    d[0] = 4;
    for (idx_t it = 2; it <= n; it++) {
        size_t ncap = 2 * len + 2;
        unsigned char *nd = (unsigned char *)malloc(ncap);
        size_t nl = 0, j = 0;
        while (j < len) {
            unsigned char dg = d[j];
            size_t c = 1;
            while (j + c < len && d[j + c] == dg) c++;
            /* counts never exceed 3 in this sequence, but handle generally */
            if (c >= 10) {
                char tmp[24];
                int t = 0;
                size_t cc = c;
                while (cc) { tmp[t++] = (char)(cc % 10); cc /= 10; }
                if (nl + t + 1 > ncap) {
                    ncap = (nl + t + 1) * 2;
                    nd = (unsigned char *)realloc(nd, ncap);
                }
                while (t) nd[nl++] = (unsigned char)tmp[--t];
            } else {
                nd[nl++] = (unsigned char)c;
            }
            nd[nl++] = dg;
            j += c;
        }
        free(d);
        d = nd;
        len = nl;
        (void)cap;
    }
    seq_t result = (seq_t)0;
    uint64_t chunk = 0;
    int k = 0;
    for (size_t i = 0; i < len; i++) {
        chunk = chunk * 10 + d[i];
        k++;
        if (k == 9) {
            result = result * (seq_t)1000000000u + (seq_t)chunk;
            chunk = 0;
            k = 0;
        }
    }
    if (k > 0) {
        uint64_t p = 1;
        for (int i = 0; i < k; i++) p *= 10;
        result = result * (seq_t)p + (seq_t)chunk;
    }
    free(d);
    return result;
}
