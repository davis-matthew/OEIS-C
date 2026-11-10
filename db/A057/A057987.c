/*
 * A057987  Positions of 1 in A057985.
 * https://oeis.org/A057987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th 1 in A057985, the fixed point of
 *            0->01, 1->12, 2->0 starting from 0 (computed by iterating the
 *            morphism)
 * Range:     u8 n <= 110, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 8769 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static unsigned char *A057987_w = NULL;
static uint64_t A057987_len = 0;
static uint64_t A057987_ones = 0;

static void A057987_expand(void)
{
    uint64_t i, j = 0, nl = 0, ones = 0;
    unsigned char *nw;
    for (i = 0; i < A057987_len; i++)
        nl += (A057987_w[i] == 2) ? 1 : 2;
    nw = (unsigned char *)malloc((size_t)nl);
    for (i = 0; i < A057987_len; i++) {
        unsigned char v = A057987_w[i];
        if (v == 0) { nw[j++] = 0; nw[j++] = 1; }
        else if (v == 1) { nw[j++] = 1; nw[j++] = 2; }
        else { nw[j++] = 0; }
    }
    for (i = 0; i < nl; i++)
        if (nw[i] == 1) ones++;
    free(A057987_w);
    A057987_w = nw;
    A057987_len = nl;
    A057987_ones = ones;
}

seq_t a(idx_t n)
{
    uint64_t i, cnt = 0;
    if (A057987_w == NULL) {
        A057987_w = (unsigned char *)malloc(1);
        A057987_w[0] = 0;
        A057987_len = 1;
        A057987_ones = 0;
    }
    while (A057987_ones < (uint64_t)n)
        A057987_expand();
    for (i = 0; i < A057987_len; i++) {
        if (A057987_w[i] == 1) {
            cnt++;
            if (cnt == (uint64_t)n)
                return (seq_t)(i + 1);
        }
    }
    return (seq_t)0;
}
