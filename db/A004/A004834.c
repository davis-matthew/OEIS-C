/*
 * A004834  Numbers that are the sum of at most 5 nonzero 4th powers.
 * https://oeis.org/A004834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve -- a number x is a term iff the minimal number of nonzero
 *            4th powers summing to x (DP: c(0)=0, c(x)=1+min_{k^4<=x}
 *            c(x-k^4)) is at most 5; terms listed in increasing order
 * Range:     u8 n <= 49, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *terms = NULL;
static uint64_t nterms = 0;
static uint64_t limit = 0;

static void build(uint64_t lim)
{
    unsigned char *c = (unsigned char *)malloc((size_t)(lim + 1));
    uint64_t *t;
    uint64_t cnt = 0, x, k;
    c[0] = 0;
    for (x = 1; x <= lim; x++) {
        unsigned char best = 255;
        for (k = 1; k * k * k * k <= x; k++) {
            unsigned char v = c[x - k * k * k * k];
            if (v < best) best = v;
        }
        c[x] = (best >= 254) ? 255 : (unsigned char)(best + 1);
        if (c[x] > 6) c[x] = 6; /* anything above 5 is just "not a term" */
    }
    for (x = 0; x <= lim; x++)
        if (c[x] <= 5) cnt++;
    t = (uint64_t *)malloc((size_t)(cnt ? cnt : 1) * sizeof(uint64_t));
    cnt = 0;
    for (x = 0; x <= lim; x++)
        if (c[x] <= 5) t[cnt++] = x;
    free(c);
    free(terms);
    terms = t;
    nterms = cnt;
    limit = lim;
}

seq_t a(idx_t n)
{
    while (nterms < n) {
        build(limit ? limit * 2 : 4096);
    }
    return (seq_t)terms[n - 1];
}
