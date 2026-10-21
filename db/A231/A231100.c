/*
 * A231100  Even legs of primitive Pythagorean triples (with multiplicity)
 *          sorted with respect to increasing hypotenuse.
 * https://oeis.org/A231100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1593, u64 n <= 1593, u128 n <= 1593,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A231100: Even legs of primitive Pythagorean triples sorted by increasing
   hypotenuse, ties broken by increasing even leg.
   Computation uses Euclid's formula, generates all primitive triples up to a
   bound that covers at least 10000 terms, sorts them, and caches the even legs
   for fast O(1) lookup. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper for GCD on 64‑bit indices. */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) { idx_t t = a % b; a = b; b = t; }
    return a;
}

/* Triple structure: hypotenuse and even leg. */
struct Triple { seq_t c; seq_t b; };

/* Comparator for qsort: primary key c, secondary key b. */
static int cmp_triple(const void *p1, const void *p2)
{
    const struct Triple *t1 = (const struct Triple *)p1;
    const struct Triple *t2 = (const struct Triple *)p2;
    if (t1->c < t2->c) return -1;
    if (t1->c > t2->c) return 1;
    if (t1->b < t2->b) return -1;
    if (t1->b > t2->b) return 1;
    return 0;
}

/* Global cache of even legs, generated lazily. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static bool initialized = false;

/* Generate enough triples to cover at least 10000 terms. */
static void generate_all(void)
{
    /* Conservative bound for m: 600 gives >90 000 primitive triples. */
    const idx_t M_MAX = 600;
    const size_t TRIPLE_CAP = 300000;
    struct Triple *triples = (struct Triple *)malloc(TRIPLE_CAP * sizeof(*triples));
    size_t len = 0;

    for (idx_t m = 2; m <= M_MAX; ++m) {
        for (idx_t n = 1; n < m; ++n) {
            if (((m - n) & 1) == 0) continue;          /* opposite parity */
            if (gcd_idx(m, n) != 1) continue;          /* primitive */

            seq_t c = (seq_t)m * m + (seq_t)n * n;
            seq_t b = (seq_t)2 * m * n;

            if (len >= TRIPLE_CAP) break;              /* safety */
            triples[len].c = c;
            triples[len].b = b;
            ++len;
        }
    }

    qsort(triples, len, sizeof(*triples), cmp_triple);

    cache_len = len;
    cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
    for (size_t i = 0; i < len; ++i) {
        cache[i] = triples[i].b;
    }

    free(triples);
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (!initialized) {
        generate_all();
        initialized = true;
    }
    /* 1‑indexed input; cast to size_t for array indexing. */
    return cache[(size_t)(n - 1)];
}
