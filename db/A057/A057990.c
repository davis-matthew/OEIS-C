/*
 * A057990  Positions of 00 in A057985.
 * https://oeis.org/A057990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position (1-based) of the n-th occurrence of "00" in
 *            A057985, the fixed point of 0->01, 1->12, 2->0 starting from 0
 * Range:     u8 n <= 24, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *A057990_w = NULL;
static idx_t A057990_len = 0;

static void A057990_expand(void)
{
    idx_t i, j = 0, nl = 0;
    unsigned char *nw;
    for (i = 0; i < A057990_len; i++)
        nl += (A057990_w[i] == 2) ? 1 : 2;
    nw = (unsigned char *)malloc((size_t)nl);
    for (i = 0; i < A057990_len; i++) {
        unsigned char v = A057990_w[i];
        if (v == 0) { nw[j++] = 0; nw[j++] = 1; }
        else if (v == 1) { nw[j++] = 1; nw[j++] = 2; }
        else { nw[j++] = 0; }
    }
    free(A057990_w);
    A057990_w = nw;
    A057990_len = nl;
}

seq_t a(idx_t n)
{
    if (A057990_w == NULL) {
        A057990_w = (unsigned char *)malloc(1);
        A057990_w[0] = 0;
        A057990_len = 1;
    }
    for (;;) {
        idx_t i, cnt = 0;
        for (i = 0; i + 1 < A057990_len; i++) {
            if (A057990_w[i] == 0 && A057990_w[i + 1] == 0) {
                cnt++;
                if (cnt == n) return (seq_t)(i + 1);
            }
        }
        A057990_expand();
    }
}
