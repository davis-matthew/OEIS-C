/*
 * A003219  Self numbers divisible by sum of their digits (or, self numbers
 *          which are also Harshad numbers).
 * https://oeis.org/A003219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search -- a(n) is the n-th number m that is a self number (no x
 *            with x + digitsum(x) = m) and is divisible by digitsum(m); found
 *            with a sliding sieve that marks x + digitsum(x)
 * Range:     u8 n <= 13, u16 n <= 885, u32 n <= 7045, u64 n <= 7045,
 *            u128 n <= 7045, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static uint64_t *terms = NULL;
static idx_t nterms = 0, cap = 0;
static uint64_t cur = 0;          /* next number to examine */
static int dig[24];               /* decimal digits of cur, least significant first */
static int ndig = 0;
static unsigned dsum = 0;         /* digit sum of cur */
static unsigned char mark[256];   /* circular buffer: mark[v & 255] set if v = x + s(x), x < v */

static void inc_cur(void) {
    int i = 0;
    cur++;
    while (1) {
        if (i == ndig) { dig[ndig++] = 0; }
        if (dig[i] < 9) { dig[i]++; dsum++; break; }
        dsum -= 9;
        dig[i] = 0;
        i++;
    }
}

seq_t a(idx_t n) {
    if (cur == 0) {
        /* x = 0 generates 0 only; begin at 1 */
        int i;
        for (i = 0; i < 256; i++) mark[i] = 0;
        cur = 1; ndig = 1; dig[0] = 1; dsum = 1;
    }
    while (nterms < n) {
        uint64_t m = cur;
        unsigned slot = (unsigned)(m & 255u);
        bool self = !mark[slot];
        mark[slot] = 0;
        if (self && m % dsum == 0) {
            if (nterms == cap) {
                idx_t nc = cap ? cap * 2 : 1024;
                terms = (uint64_t *)realloc(terms, nc * sizeof(uint64_t));
                cap = nc;
            }
            terms[nterms++] = m;
        }
        mark[(unsigned)((m + dsum) & 255u)] = 1;
        inc_cur();
    }
    return (seq_t)terms[n - 1];
}
