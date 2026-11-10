/*
 * A057986  Positions of 0 in A057985.
 * https://oeis.org/A057986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th 0 in A057985, the fixed point of
 *            0->01, 1->12, 2->0 (computed by iterating the morphism)
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 8845 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *A057986_w = NULL;
static size_t A057986_len = 0;
static idx_t *A057986_pos = NULL;
static size_t A057986_cnt = 0;

seq_t a(idx_t n)
{
    if (A057986_w == NULL) {
        A057986_w = (unsigned char *)malloc(1);
        A057986_w[0] = 0;
        A057986_len = 1;
    }
    while (A057986_cnt < (size_t)n) {
        size_t nl = 0;
        for (size_t i = 0; i < A057986_len; i++)
            nl += (A057986_w[i] == 2) ? 1 : 2;
        unsigned char *nw = (unsigned char *)malloc(nl);
        size_t j = 0;
        for (size_t i = 0; i < A057986_len; i++) {
            unsigned char v = A057986_w[i];
            if (v == 0) { nw[j++] = 0; nw[j++] = 1; }
            else if (v == 1) { nw[j++] = 1; nw[j++] = 2; }
            else nw[j++] = 0;
        }
        free(A057986_w);
        A057986_w = nw;
        A057986_len = nl;
        size_t c = 0;
        for (size_t i = 0; i < nl; i++)
            if (nw[i] == 0) c++;
        free(A057986_pos);
        A057986_pos = (idx_t *)malloc(c * sizeof(idx_t));
        c = 0;
        for (size_t i = 0; i < nl; i++)
            if (nw[i] == 0) A057986_pos[c++] = (idx_t)(i + 1);
        A057986_cnt = c;
    }
    return (seq_t)A057986_pos[n - 1];
}
