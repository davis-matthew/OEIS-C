/*
 * A057989  Positions of 00 or 11 or 22 in A057985.
 * https://oeis.org/A057989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position i of the n-th pair with w(i)=w(i+1) in A057985,
 *            the fixed point of 0->01, 1->12, 2->0 starting from 0 (computed
 *            by expanding the word)
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *A057989_w = NULL;
static idx_t A057989_len = 0;

static void A057989_expand(void)
{
    idx_t newlen = 0;
    for (idx_t i = 0; i < A057989_len; ++i)
        newlen += (A057989_w[i] == 2) ? 1 : 2;
    unsigned char *nw = (unsigned char *)malloc((size_t)newlen);
    idx_t j = 0;
    for (idx_t i = 0; i < A057989_len; ++i) {
        unsigned char v = A057989_w[i];
        if (v == 0) { nw[j++] = 0; nw[j++] = 1; }
        else if (v == 1) { nw[j++] = 1; nw[j++] = 2; }
        else { nw[j++] = 0; }
    }
    free(A057989_w);
    A057989_w = nw;
    A057989_len = newlen;
}

seq_t a(idx_t n)
{
    if (A057989_w == NULL) {
        A057989_w = (unsigned char *)malloc(1);
        A057989_w[0] = 0;
        A057989_len = 1;
    }
    for (;;) {
        idx_t cnt = 0;
        for (idx_t i = 0; i + 1 < A057989_len; ++i) {
            if (A057989_w[i] == A057989_w[i + 1]) {
                ++cnt;
                if (cnt == n)
                    return (seq_t)(i + 1);
            }
        }
        A057989_expand();
    }
}
