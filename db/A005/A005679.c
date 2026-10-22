/*
 * A005679  A squarefree (or Thue-Morse) ternary sequence: closed under
 *          a->abc, b->ac, c->b.
 * https://oeis.org/A005679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n-th term of the fixed point, starting with 2, of the
 *            morphism 1 -> 23, 2 -> 213, 3 -> 1 (computed by iterating the
 *            morphism)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static unsigned char *A005679_buf = NULL;
static idx_t A005679_len = 0;

static void A005679_build(idx_t n) {
    if (A005679_buf == NULL) {
        A005679_buf = (unsigned char *) malloc(1);
        A005679_buf[0] = 2;
        A005679_len = 1;
    }
    while (A005679_len < n) {
        idx_t new_len = 0;
        idx_t i;
        for (i = 0; i < A005679_len; i++) {
            unsigned char c = A005679_buf[i];
            new_len += (c == 1) ? 2 : (c == 2) ? 3 : 1;
        }
        unsigned char *t = (unsigned char *) malloc((size_t) new_len);
        idx_t pos = 0;
        for (i = 0; i < A005679_len; i++) {
            unsigned char c = A005679_buf[i];
            if (c == 1) {
                t[pos++] = 2; t[pos++] = 3;
            } else if (c == 2) {
                t[pos++] = 2; t[pos++] = 1; t[pos++] = 3;
            } else {
                t[pos++] = 1;
            }
        }
        free(A005679_buf);
        A005679_buf = t;
        A005679_len = new_len;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    A005679_build(n);
    return (seq_t) A005679_buf[n - 1];
}
