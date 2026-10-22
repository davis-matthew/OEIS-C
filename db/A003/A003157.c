/*
 * A003157  A self-generating sequence (see Comments in A003156 for the
 *          definition).
 * https://oeis.org/A003157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th 'c' in the fixed point of the
 *            morphism a -> abc, b -> a, c -> a starting with a; the word is
 *            built by expanding itself letter by letter
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *w3157 = NULL;
static size_t w3157_len = 0, w3157_cap = 0, w3157_rd = 0;
static uint64_t *p3157 = NULL;
static size_t p3157_cnt = 0, p3157_cap = 0;

static void w3157_push(unsigned char ch) {
    if (w3157_len == w3157_cap) {
        size_t nc = w3157_cap ? w3157_cap * 2 : 1024;
        w3157 = (unsigned char *)realloc(w3157, nc);
        w3157_cap = nc;
    }
    w3157[w3157_len++] = ch;
    if (ch == 2) {
        if (p3157_cnt == p3157_cap) {
            size_t nc = p3157_cap ? p3157_cap * 2 : 256;
            p3157 = (uint64_t *)realloc(p3157, nc * sizeof(uint64_t));
            p3157_cap = nc;
        }
        p3157[p3157_cnt++] = (uint64_t)w3157_len; /* 1-based position */
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (w3157_len == 0) {
        /* image of the first letter 'a' is "abc"; 0=a, 1=b, 2=c */
        w3157_push(0);
        w3157_push(1);
        w3157_push(2);
        w3157_rd = 1;
    }
    while (p3157_cnt < n) {
        unsigned char ch = w3157[w3157_rd++];
        if (ch == 0) {
            w3157_push(0);
            w3157_push(1);
            w3157_push(2);
        } else {
            w3157_push(0);
        }
    }
    return (seq_t)p3157[n - 1];
}
