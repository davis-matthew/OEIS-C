/*
 * A001155  Describe the previous term! (method A - initial term is 0).
 * https://oeis.org/A001155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(1) = 0; a(n) = look-and-say description of a(n-1) (method A:
 *            count followed by digit)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static unsigned char *ls_digits = NULL;
static size_t ls_len = 0;
static idx_t ls_n = 0;

static void ls_reset(void) {
    free(ls_digits);
    ls_digits = (unsigned char *)malloc(16);
    ls_digits[0] = 0;
    ls_len = 1;
    ls_n = 1;
}

static void ls_step(void) {
    size_t cap = ls_len * 2 + 16;
    unsigned char *next = (unsigned char *)malloc(cap);
    size_t nl = 0;
    size_t i = 0;
    while (i < ls_len) {
        unsigned char d = ls_digits[i];
        size_t cnt = 1;
        while (i + cnt < ls_len && ls_digits[i + cnt] == d) cnt++;
        {
            unsigned char buf[24];
            int bl = 0;
            size_t c = cnt;
            while (c > 0) { buf[bl++] = (unsigned char)(c % 10); c /= 10; }
            if (nl + bl + 1 > cap) {
                cap = cap * 2 + bl + 1;
                next = (unsigned char *)realloc(next, cap);
            }
            while (bl > 0) next[nl++] = buf[--bl];
        }
        next[nl++] = d;
        i += cnt;
    }
    free(ls_digits);
    ls_digits = next;
    ls_len = nl;
    ls_n++;
}

seq_t a(idx_t n) {
    seq_t result = 0;
    size_t i;
    if (ls_digits == NULL || n < ls_n) ls_reset();
    while (ls_n < n) ls_step();
    for (i = 0; i < ls_len; i++) {
        result = result * 10 + (seq_t)ls_digits[i];
    }
    return result;
}
