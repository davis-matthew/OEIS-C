/*
 * A007621  Impractical numbers: even abundant numbers (A173490) that are not
 *          practical(2) (A007620).
 * https://oeis.org/A007621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over even m, keeping m with sigma(m) > 2m (abundant) that
 *            fail Stewart's practical-number test (m = 2^a1*p2^a2*... with
 *            p_{i+1} <= 1 + sigma(p1^a1*...*pi^ai))
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_impractical_abundant
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_impractical_abundant.h"

static uint64_t *terms = NULL;
static idx_t nterms = 0, capterms = 0;
static uint64_t last_checked = 0;

seq_t a(idx_t n) {
    while (nterms < n) {
        uint64_t m = last_checked + 2;
        last_checked = m;
        if (is_impractical_abundant(m)) {
            if (nterms == capterms) {
                idx_t nc = capterms ? capterms * 2 : 1024;
                terms = (uint64_t *)realloc(terms, nc * sizeof(uint64_t));
                capterms = nc;
            }
            terms[nterms++] = m;
        }
    }
    return (seq_t)terms[n - 1];
}
