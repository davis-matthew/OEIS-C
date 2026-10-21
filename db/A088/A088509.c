/*
 * A088509  Short leg of primitive Pythagorean triangles sorted on product of
 *          sides.
 * https://oeis.org/A088509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 none, u32 n <= 23, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Short leg of primitive Pythagorean triangles sorted by product of sides.
   The function generates primitive triples (a,b,c), sorts them by a*b*c,
   and returns the short leg (min(a,b)) of the n‑th triple in that order. */

#include <stdlib.h>
#include "../utils/gcd.h"

typedef struct {
    seq_t prod;
    seq_t shortleg;
} Triple;

static Triple *triples = NULL;
static size_t triple_len = 0;
static size_t triple_cap = 0;
static idx_t m_max = 0;          /* largest m processed */

static int cmp_triple(const void *p, const void *q) {
    const Triple *a = (const Triple *)p;
    const Triple *b = (const Triple *)q;
    if (a->prod < b->prod) return -1;
    if (a->prod > b->prod) return 1;
    if (a->shortleg < b->shortleg) return -1;
    if (a->shortleg > b->shortleg) return 1;
    return 0;
}

/* Expand storage if needed. */
static void ensure_capacity(size_t needed, size_t *cap) {
    if (needed <= *cap) return;
    size_t new_cap = *cap ? *cap * 2 : 32;
    while (new_cap < needed) new_cap *= 2;
    triples = (Triple *)realloc(triples, new_cap * sizeof(Triple));
    *cap = new_cap;
}

/* Grow the list of primitive triples until at least n entries are sorted. */
static void ensure_triples(idx_t n) {
    if (n <= (idx_t)triple_len && m_max) return;  /* already enough */

    /* Increase m range in blocks (e.g., 100) to cover more triples. */
    const idx_t block = 100;
    while ((idx_t)triple_len < n) {
        idx_t new_m_max = m_max + block;

        for (idx_t m = m_max + 1; m <= new_m_max; ++m) {
            for (idx_t cli = 1; cli < m; ++cli) {
                if (((m - cli) & 1U) == 0) continue;          /* parity condition */
                if (gcd((seq_t)m, (seq_t)cli) != 1) continue;  /* coprime condition */

                seq_t mm = (seq_t)m * (seq_t)m;
                seq_t nn = (seq_t)cli * (seq_t)cli;
                seq_t a = mm - nn;                               /* m^2-n^2 */
                seq_t b = (seq_t)2 * (seq_t)m * (seq_t)cli;      /* 2mn */
                seq_t c = mm + nn;                               /* m^2+n^2 */
                seq_t prod = a * b * c;

                Triple t = { prod, (a < b) ? a : b };
                ensure_capacity(triple_len + 1, &triple_cap);
                triples[triple_len++] = t;
            }
        }

        m_max = new_m_max;
        /* Re‑sort after adding new triples. */
        qsort(triples, triple_len, sizeof(Triple), cmp_triple);
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    ensure_triples(n);
    return triples[n - 1].shortleg;
}
