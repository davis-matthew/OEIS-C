/*
 * A102827  "True already", base 10, start 1: a(n) is the least integer such
 *          that the sequence up to a(n-1) written in base 10 contains
 *          floor(a(n)/10) copies of the digit a(n) % 10, with a(0) = 1.
 * https://oeis.org/A102827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = least integer x > 0 such that the number of occurrences
 *            of digit (x % 10) in the decimal concatenation of a(0)…a(n-1)
 *            equals floor(x / 10)
 * Range:     u8 n <= 60, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* static cache state */
    static seq_t *terms = NULL;
    static size_t size = 0;          /* number of terms stored (>=1 after init) */
    static size_t cap = 0;           /* allocated capacity */
    static seq_t counts[10] = {0};   /* digit frequency 0..9 */
    static bool initialized = false;

    if (!initialized) {
        /* first term a(0)=1 */
        cap = 1;
        terms = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        terms[0] = 1;
        size = 1;
        for (int d = 0; d < 10; ++d) counts[d] = 0;
        counts[1] = 1;               /* digit '1' occurs once in the term 1 */
        initialized = true;
    }

    /* grow cache if needed */
    if (n >= cap) {
        size_t new_cap = (n + 1);
        terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!terms) abort();        /* allocation failure */
        cap = new_cap;
    }

    /* generate missing terms up to n */
    for (size_t i = size; i <= n; ++i) {
        /* find least candidate > 0 */
        seq_t best = 0;
        bool first = true;
        for (int d = 0; d < 10; ++d) {
            seq_t cand = (seq_t)10 * counts[d] + d;
            if (cand == 0) continue;   /* skip 0 */
            if (first || cand < best) {
                best = cand;
                first = false;
            }
        }
        terms[i] = best;

        /* update digit counts for the new term */
        seq_t t = best;
        if (t == 0) {                  /* should not happen, but be safe */
            counts[0]++;
        } else {
            while (t > 0) {
                int d = (int)(t % 10);
                counts[d] += 1;
                t /= 10;
            }
        }
    }

    size = n + 1;   /* we've now stored up to index n */
    return terms[n];
}
