/*
 * A001391  To get the 3rd term, for example, note that 2nd term has three (11
 *          in binary!) 1's and one (1) 0, giving 11 1 1 0.
 * https://oeis.org/A001391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 10; a(n) = binary look-and-say of a(n-1): read each
 *            maximal run of equal digits of a(n-1) as (run length written in
 *            binary) followed by the digit, then concatenate the results
 *            (A001391)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    size_t len = 2;
    unsigned char *cur = (unsigned char *)malloc(len);
    cur[0] = 1;
    cur[1] = 0;

    for (idx_t iter = 2; iter <= n; iter++) {
        /* each run of length r emits bitlen(r)+1 <= 2r digits */
        unsigned char *nxt = (unsigned char *)malloc(2 * len + 8);
        size_t nl = 0;
        size_t pos = 0;
        while (pos < len) {
            unsigned char d = cur[pos];
            size_t run = 1;
            while (pos + run < len && cur[pos + run] == d) run++;
            /* write run in binary, most significant bit first */
            int bl = 0;
            size_t t = run;
            while (t > 0) { bl++; t >>= 1; }
            for (int b = bl - 1; b >= 0; b--)
                nxt[nl++] = (unsigned char)((run >> b) & 1);
            nxt[nl++] = d;
            pos += run;
        }
        free(cur);
        cur = nxt;
        len = nl;
    }

    seq_t result = 0;
    for (size_t i = 0; i < len; i++) {
        result = result * 10 + (seq_t)cur[i];
    }
    free(cur);
    return result;
}
