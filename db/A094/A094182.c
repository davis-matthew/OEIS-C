/*
 * A094182  Inradii of primitive Pythagorean triangles, sorted on area
 *          A024406(n), then on inradii.
 * https://oeis.org/A094182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 23, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * This implementation generates all primitive Pythagorean triples
 * using Euclid's formula (m>n, gcd(m,n)=1, m-n odd), computes
 * their area A = mn(m²−n²) and inradius r = n(m−n), sorts the
 * triples first by area and then by r, and stores the sorted
 * inradii in a cache.  a(n) returns the (n‑1)‑th element of this
 * cache (1‑based index).  The cache is built lazily on first use.
 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

struct Entry
{
    seq_t area;
    seq_t r;
};

static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static bool initialized  = false;

/* Comparator used with qsort to sort by area, then by r. */
static int cmp_entries(const void *p1, const void *p2)
{
    const struct Entry *e1 = (const struct Entry *)p1;
    const struct Entry *e2 = (const struct Entry *)p2;
    if (e1->area < e2->area) return -1;
    if (e1->area > e2->area) return  1;
    if (e1->r   < e2->r)    return -1;
    if (e1->r   > e2->r)    return  1;
    return 0;
}

/* Build the cache of inradii.  The limit on m is chosen so that
 * the cache contains at least a few hundred thousand entries
 * (enough for n up to ~10⁵).  The algorithm is O(M²). */
static void init_cache(void)
{
    if (initialized) return;

    /* Upper bound on m; 2000 gives ~3‑4×10⁵ primitive triples. */
    const idx_t MMAX = 2000;

    size_t capacity = 100000;                        /* initial capacity */
    struct Entry *entries = (struct Entry *)malloc(capacity * sizeof(struct Entry));
    size_t len = 0;

    for (idx_t m = 2; m <= MMAX; ++m)
    {
        for (idx_t n = 1; n < m; ++n)
        {
            if (((m - n) & 1) == 0) continue;        /* m-n must be odd */
            if (gcd((seq_t)m, (seq_t)n) != (seq_t)1) continue; /* coprime */

            seq_t m2 = (seq_t)m * (seq_t)m;
            seq_t n2 = (seq_t)n * (seq_t)n;
            seq_t area = (seq_t)m * (seq_t)n * (m2 - n2);
            seq_t r    = (seq_t)n * ((seq_t)m - (seq_t)n);

            if (len == capacity)
            {
                capacity <<= 1;
                entries = (struct Entry *)realloc(entries, capacity * sizeof(struct Entry));
            }
            entries[len].area = area;
            entries[len].r    = r;
            ++len;
        }
    }

    qsort(entries, len, sizeof(struct Entry), cmp_entries);

    cache = (seq_t *)malloc(len * sizeof(seq_t));
    for (size_t i = 0; i < len; ++i) cache[i] = entries[i].r;
    cache_len = len;

    free(entries);
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init_cache();
    if (n == 0 || n > cache_len) return 0;
    return cache[n - 1];
}
