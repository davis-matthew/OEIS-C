/*
 * A001387  Binary "look and say" sequence.
 * https://oeis.org/A001387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1; a(n) is the binary look-and-say description of a(n-1):
 *            each run of k equal digits d becomes (k written in binary)
 *            followed by d. Each term is a string of 0s and 1s, read as a
 *            decimal number
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    size_t len = 1, cap = 16;
    char *cur = (char *)malloc(cap);
    cur[0] = '1';
    for (idx_t i = 2; i <= n; i++) {
        /* each run of length k produces at most bitlen(k)+1 <= k+1 chars, so 2*len suffices */
        size_t ncap = 2 * len + 2;
        char *nxt = (char *)malloc(ncap);
        size_t nl = 0, pos = 0;
        while (pos < len) {
            char d = cur[pos];
            size_t k = 1;
            while (pos + k < len && cur[pos + k] == d) k++;
            int bl = 0;
            size_t t = k;
            while (t) { bl++; t >>= 1; }
            for (int j = bl - 1; j >= 0; j--)
                nxt[nl++] = (char)('0' + ((k >> j) & 1));
            nxt[nl++] = d;
            pos += k;
        }
        free(cur);
        cur = nxt;
        len = nl;
        cap = ncap;
    }
    (void)cap;
    seq_t r = 0;
    for (size_t i = 0; i < len; i++)
        r = r * 10 + (seq_t)(cur[i] - '0');
    free(cur);
    return r;
}
