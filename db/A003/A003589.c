/*
 * A003589  a(n) has the property that the sequence b(n) = number of 2's
 *          between successive 3's is the same as the original sequence.
 * https://oeis.org/A003589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   self-generating sequence: a(0..2) = 2,2,3; then for k =
 *            0,1,2,... append a(k) copies of 2 followed by one 3
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static unsigned char *a003589_buf = NULL;
static size_t a003589_len = 0, a003589_cap = 0, a003589_k = 0;

static void a003589_push(unsigned char v) {
    if (a003589_len == a003589_cap) {
        size_t nc = a003589_cap ? a003589_cap * 2 : 64;
        a003589_buf = (unsigned char *)realloc(a003589_buf, nc);
        a003589_cap = nc;
    }
    a003589_buf[a003589_len++] = v;
}

seq_t a(idx_t n) {
    if (a003589_len == 0) {
        a003589_push(2);
        a003589_push(2);
        a003589_push(3);
    }
    while ((idx_t)a003589_len <= n) {
        unsigned char c = a003589_buf[a003589_k++];
        unsigned char i;
        for (i = 0; i < c; i++) a003589_push(2);
        a003589_push(3);
    }
    return (seq_t)a003589_buf[n];
}
