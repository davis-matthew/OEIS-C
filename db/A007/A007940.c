/*
 * A007940  Noncubes such that some permutation of digits is a cube.
 * https://oeis.org/A007940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search: a(n) = n-th noncube x such that the multiset of nonzero
 *            digits of x equals that of some cube c < 10^(number of digits of
 *            x) (permutations may have leading zeros)
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a7940_cmp, a7940_sig
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/a7940_cmp.h"
#include "../utils/a7940_sig.h"

static uint64_t *a7940_terms = NULL;
static size_t a7940_nterms = 0, a7940_cap = 0;
static uint64_t a7940_x = 0;      /* last number scanned */
static uint64_t a7940_lim = 1;    /* 10^d, current digit-length bound */
static uint64_t *a7940_sigs = NULL;
static size_t a7940_nsigs = 0;
static uint64_t a7940_ck = 1;     /* cube-root cursor */

static void a7940_rebuild(void) {
    size_t cnt = 0, k;
    for (k = 1; (uint64_t)k * k * k < a7940_lim; k++) cnt++;
    free(a7940_sigs);
    a7940_sigs = (uint64_t *)malloc((cnt + 1) * sizeof(uint64_t));
    a7940_nsigs = 0;
    for (k = 1; (uint64_t)k * k * k < a7940_lim; k++)
        a7940_sigs[a7940_nsigs++] = a7940_sig((uint64_t)k * k * k);
    qsort(a7940_sigs, a7940_nsigs, sizeof(uint64_t), a7940_cmp);
}

seq_t a(idx_t n) {
    while (a7940_nterms < n) {
        uint64_t x = ++a7940_x, s;
        if (x >= a7940_lim) {
            a7940_lim *= 10;
            a7940_rebuild();
        }
        while (a7940_ck * a7940_ck * a7940_ck < x) a7940_ck++;
        if (a7940_ck * a7940_ck * a7940_ck == x) continue;
        s = a7940_sig(x);
        if (bsearch(&s, a7940_sigs, a7940_nsigs, sizeof(uint64_t), a7940_cmp)) {
            if (a7940_nterms == a7940_cap) {
                a7940_cap = a7940_cap ? 2 * a7940_cap : 1024;
                a7940_terms = (uint64_t *)realloc(a7940_terms, a7940_cap * sizeof(uint64_t));
            }
            a7940_terms[a7940_nterms++] = x;
        }
    }
    return (seq_t)a7940_terms[n - 1];
}
