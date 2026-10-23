/*
 * A119286  Alternating sum of the fifth powers of the first n Fibonacci
 *          numbers.
 * https://oeis.org/A119286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=0, a(1)=-1, a(2)=0, a(3)=-32, a(4)=211, a(5)=-2914, *
 *            a(6)=29854, a(7)=-341439. * Recurrence:
 *            a(n)+7a(n‑1)–48a(n‑2)–20a(n‑3)+100a(n‑4)–32a(n‑5)–9a(n‑6)+a(n‑7)=0
 *            * for n≥8. The sequence is memoised for arbitrary‑order access
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 10, i64 n <= 19, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t init[8] = {
    (seq_t)0,   (seq_t)-1, (seq_t)0,  (seq_t)-32,
    (seq_t)211, (seq_t)-2914, (seq_t)29854, (seq_t)-341439
};

static seq_t *cache = NULL;  /* memoised terms */
static size_t  cap   = 0;    /* allocated capacity */
static size_t  sz    = 0;    /* number of stored terms */

seq_t a(idx_t n)
{
    /* return a cached value if available */
    if (n < sz) return cache[(size_t)n];

    /* ensure we have enough space */
    size_t need = (size_t)n + 1;
    if (!cache) {
        cap = need > 8 ? need : 8;
        cache = (seq_t*)calloc(cap, sizeof(seq_t));
    } else if (need > cap) {
        size_t new_cap = cap == 0 ? 8 : cap * 2;
        while (new_cap < need) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }

    /* initialise the first eight terms once */
    if (sz == 0) {
        for (int i = 0; i < 8; ++i) cache[i] = init[i];
        sz = 8;
    }

    /* compute terms up to n */
    while (sz <= n) {
        idx_t i = sz;
        seq_t val;
        if (i <= 7) {
            val = init[i];
        } else {
            /* recurrence: a[i] = -7*a[i-1] + 48*a[i-2] + 20*a[i-3] - 100*a[i-4]
                           + 32*a[i-5] + 9*a[i-6] - a[i-7] */
            val = -(cache[i-1] * (seq_t)7)
                + (cache[i-2] * (seq_t)48)
                + (cache[i-3] * (seq_t)20)
                - (cache[i-4] * (seq_t)100)
                + (cache[i-5] * (seq_t)32)
                + (cache[i-6] * (seq_t)9)
                - cache[i-7];
        }
        cache[sz] = val;
        ++sz;
    }

    return cache[(size_t)n];
}
