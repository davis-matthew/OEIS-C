/*
 * A180620  Odd legs of primitive Pythagorean triples (with multiplicity)
 *          sorted with respect to increasing hypotenuse.
 * https://oeis.org/A180620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = the odd leg of the n‑th primitive Pythagorean triple,
 *            sorted by increasing hypotenuse c, ties broken by increasing
 *            even leg b (Euclid's formula: a = p² − q², b = 2pq, c = p² + q²,
 *            p>q>0, coprime and of opposite parity)
 * Range:     u8 none, u16 n <= 1593, u32 n <= 1593, u64 n <= 1593,
 *            u128 n <= 1593, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  C++ and C compilation are supported.  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

#ifdef __cplusplus
#include <vector>
#include <algorithm>
#endif

/* Triple representation. */
typedef struct {
    seq_t a, b, c;
} triple_t;

/* Comparison for sorting by c then b. */
static int cmp_triple(const void *p1, const void *p2)
{
    const triple_t *t1 = (const triple_t *)p1;
    const triple_t *t2 = (const triple_t *)p2;
    if (t1->c < t2->c) return -1;
    if (t1->c > t2->c) return 1;
    if (t1->b < t2->b) return -1;
    if (t1->b > t2->b) return 1;
    return 0;
}

#ifdef __cplusplus
/* C++ implementation using std::vector. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* unused offset */

    std::vector<triple_t> triples;
    idx_t p_lim = 100;                    /* initial search bound on p */

    while (triples.size() < n) {
        /* generate primitives with p up to p_lim */
        for (idx_t p = 2; p <= p_lim; ++p) {
            seq_t p2 = (seq_t)p * (seq_t)p;
            for (idx_t q = 1; q < p; ++q) {
                /* opposite parity and coprime */
                if (((p - q) & 1u) == 0) continue;
                if (gcd((seq_t)p, (seq_t)q) != 1) continue;

                seq_t q2 = (seq_t)q * (seq_t)q;
                triple_t t;
                t.a = p2 - q2;        /* odd leg             */
                t.b = 2 * (seq_t)p * (seq_t)q; /* even leg        */
                t.c = p2 + q2;        /* hypotenuse         */
                triples.push_back(t);
            }
        }

        /* if still not enough, enlarge search range */
        if (triples.size() < n) p_lim <<= 1;
    }

    /* sort primtives by (c,b) */
    std::sort(triples.begin(), triples.end(),
              [](const triple_t &x, const triple_t &y) {
                  if (x.c < y.c) return true;
                  if (x.c > y.c) return false;
                  return x.b < y.b;
              });

    return triples[n - 1].a;
}
#else
/* C implementation using malloc/qsort. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* unused offset */

    /* dynamic array of triples */
    size_t cap   = 16384;
    size_t count = 0;
    triple_t *triples = (triple_t *)malloc(cap * sizeof(triple_t));
    if (!triples) return 0; /* allocation failure yields 0 */

    idx_t p_lim = 100;                    /* initial bound on p */

    while (count < n) {
        /* generate primitives with p up to p_lim */
        for (idx_t p = 2; p <= p_lim; ++p) {
            seq_t p2 = (seq_t)p * (seq_t)p;
            for (idx_t q = 1; q < p; ++q) {
                /* opposite parity and coprime */
                if (((p - q) & 1u) == 0) continue;
                if (gcd((seq_t)p, (seq_t)q) != 1) continue;

                seq_t q2 = (seq_t)q * (seq_t)q;
                if (count == cap) {
                    cap <<= 1;
                    triples = (triple_t *)realloc(triples, cap * sizeof(triple_t));
                    if (!triples) return 0; /* allocation failure */
                }
                triples[count].a = p2 - q2;
                triples[count].b = 2 * (seq_t)p * (seq_t)q;
                triples[count].c = p2 + q2;
                ++count;
            }
        }

        /* if still insufficient triples, increase search range */
        if (count < n) p_lim <<= 1;
    }

    /* sort by (c,b) */
    qsort(triples, count, sizeof(triple_t), cmp_triple);

    seq_t result = triples[n - 1].a;
    free(triples);
    return result;
}
#endif
