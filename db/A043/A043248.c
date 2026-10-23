/*
 * A043248  Numbers k such that 4 and 7 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer k such that * k contains the
 *            adjacent digits 47 or 74 in base‑10 and * k−1 does not contain
 *            these adjacent digits. * This implementation linearly scans the
 *            natural numbers * until the required index is reached, caching
 *            results * for subsequent calls
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_adjacent47or74
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/has_adjacent47or74.h"

seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *terms = NULL;
    static size_t cap = 0;
    static size_t sz  = 0;   /* number of cached terms */
    static idx_t last_k = 0; /* last number examined */

    if (terms == NULL) {                     /* initialise cache */
        cap = 16;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!terms) exit(EXIT_FAILURE);
    }

    /* Generate more terms until we have at least n. */
    while (sz < n) {
        idx_t k = last_k + 1;
        seq_t sk = (seq_t)k;

        if (has_adjacent47or74(sk)) {
            seq_t sm1 = (seq_t)(k - 1);
            if (!has_adjacent47or74(sm1)) {
                if (sz == cap) {               /* grow cache if needed */
                    cap *= 2;
                    terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
                    if (!terms) exit(EXIT_FAILURE);
                }
                terms[sz++] = sk;
            }
        }
        last_k = k;
    }

    return terms[n - 1];
}
