/*
 * A003158  A self-generating sequence (see Comments in A003156 for the
 *          definition).
 * https://oeis.org/A003158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   no closed form is used. The code builds the fixed point of the
 *            morphism a -> abc, b -> a, c -> a, starting from a, and returns
 *            the 1-based position of the n-th b
 * Range:     u8 n <= 40, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static unsigned char *w3158 = NULL;
static size_t w3158_len = 0, w3158_cap = 0, w3158_rd = 0;
static idx_t *p3158 = NULL;
static size_t p3158_cnt = 0, p3158_cap = 0;

static void a3158_push(unsigned char c) {
    if (w3158_len == w3158_cap) {
        size_t nc = w3158_cap ? w3158_cap * 2 : 1024;
        w3158 = (unsigned char *)realloc(w3158, nc);
        w3158_cap = nc;
    }
    w3158[w3158_len++] = c;
    if (c == 1) {
        if (p3158_cnt == p3158_cap) {
            size_t nc = p3158_cap ? p3158_cap * 2 : 1024;
            p3158 = (idx_t *)realloc(p3158, nc * sizeof(idx_t));
            p3158_cap = nc;
        }
        p3158[p3158_cnt++] = (idx_t)w3158_len; /* 1-based position */
    }
}

seq_t a(idx_t n) {
    if (w3158 == NULL) {
        a3158_push(0);
        a3158_push(1);
        a3158_push(2);
        w3158_rd = 1;
    }
    while ((idx_t)p3158_cnt < n) {
        unsigned char c = w3158[w3158_rd++];
        if (c == 0) {
            a3158_push(0);
            a3158_push(1);
            a3158_push(2);
        } else {
            a3158_push(0);
        }
    }
    return (seq_t)p3158[n - 1];
}
