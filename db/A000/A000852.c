/*
 * A000852  Numbers beginning with a vowel in English.
 * https://oeis.org/A000852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  word, nonn, easy
 * Formula:   search: a(n) = n-th k >= 1 whose English name begins with a
 *            vowel, i.e. whose leading three-digit group (k / 1000^j, 1..999)
 *            is 1, 8, 11, 18, 80..89, or has hundreds digit 1 or 8
 * Range:     u8 n <= 114, u16 n <= 4214, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   vowel_start
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/vowel_start.h"

static uint64_t *terms = NULL;
static idx_t nterms = 0, capterms = 0;
static uint64_t last_k = 0;

seq_t a(idx_t n) {
    while (nterms < n) {
        last_k++;
        if (vowel_start(last_k)) {
            if (nterms == capterms) {
                idx_t nc = capterms ? capterms * 2 : 1024;
                uint64_t *t = (uint64_t *)realloc(terms, (size_t)nc * sizeof(uint64_t));
                if (!t) return 0;
                terms = t;
                capterms = nc;
            }
            terms[nterms++] = last_k;
        }
    }
    return (seq_t)terms[n - 1];
}
