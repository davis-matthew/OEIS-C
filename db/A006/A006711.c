/*
 * A006711  Describe previous term from the right (method A - initial term is
 *          1).
 * https://oeis.org/A006711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(1) = 1; a(n+1) = A045918(A004086(a(n))), i.e. read the digits
 *            of a(n) from right to left and write each run as count followed
 *            by digit (done on decimal digit strings)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static unsigned char *cur = NULL;   /* digits of a(cur_n), most significant first */
static size_t cur_len = 0;
static idx_t cur_n = 0;

static void reset_seq(void) {
    if (cur) free(cur);
    cur = (unsigned char *)malloc(1);
    cur[0] = 1;
    cur_len = 1;
    cur_n = 1;
}

static void step(void) {
    /* worst case: each digit becomes count+digit -> at most 2*len (counts < 10 typical; allow extra) */
    size_t cap = 2 * cur_len + 32;
    unsigned char *out = (unsigned char *)malloc(cap);
    size_t olen = 0;
    size_t i = cur_len;
    while (i > 0) {
        unsigned char d = cur[i - 1];
        size_t cnt = 0;
        while (i > 0 && cur[i - 1] == d) { cnt++; i--; }
        unsigned char tmp[24];
        int t = 0;
        while (cnt > 0) { tmp[t++] = (unsigned char)(cnt % 10); cnt /= 10; }
        if (olen + (size_t)t + 1 > cap) {
            cap = cap * 2 + (size_t)t + 1;
            out = (unsigned char *)realloc(out, cap);
        }
        while (t > 0) out[olen++] = tmp[--t];
        out[olen++] = d;
    }
    free(cur);
    cur = out;
    cur_len = olen;
    cur_n++;
}

seq_t a(idx_t n) {
    if (cur == NULL || n < cur_n) reset_seq();
    while (cur_n < n) step();
    seq_t r = 0;
    for (size_t i = 0; i < cur_len; i++) {
        r = r * 10 + (int)cur[i];
    }
    return r;
}
