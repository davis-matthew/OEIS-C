/*
 * A003340  Numbers that are the sum of 6 positive 4th powers.
 * https://oeis.org/A003340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sieve, not a closed form. A bitmask DP over x = 1..L marks which
 *            counts k <= 6 of positive 4th powers can sum to x. x is a term
 *            when k = 6 is reachable. L doubles until there are at least n
 *            terms
 * Range:     u8 n <= 19, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *terms = NULL;
static idx_t terms_len = 0;

static void build(uint64_t L) {
    unsigned char *m = (unsigned char *)malloc((size_t)(L + 1));
    uint64_t pw[256];
    int np = 0;
    uint64_t x, j;
    idx_t cnt = 0, cap = 1024;
    uint64_t *t = (uint64_t *)malloc((size_t)cap * sizeof(uint64_t));
    for (j = 1; j * j * j * j <= L; j++) pw[np++] = j * j * j * j;
    m[0] = 1;
    for (x = 1; x <= L; x++) {
        unsigned char v = 0;
        int i;
        for (i = 0; i < np && pw[i] <= x; i++)
            v |= (unsigned char)((m[x - pw[i]] << 1) & 0x7F);
        m[x] = v;
        if (v & 0x40) {
            if (cnt == cap) {
                cap *= 2;
                t = (uint64_t *)realloc(t, (size_t)cap * sizeof(uint64_t));
            }
            t[cnt++] = x;
        }
    }
    free(m);
    if (terms) free(terms);
    terms = t;
    terms_len = cnt;
}

seq_t a(idx_t n) {
    uint64_t L = 4096;
    while (terms_len < n) {
        build(L);
        L *= 2;
    }
    return (seq_t)terms[n - 1];
}
