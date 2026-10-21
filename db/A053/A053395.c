/*
 * A053395  Complete list of 4-Kaprekar numbers.
 * https://oeis.org/A053395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   a(n) = the n-th positive integer n such that (n^2 % 10000) +
 *            (n^2 / 10000) == n
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   is_4kaprekar
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_4kaprekar.h"

static seq_t *terms = NULL;          /* stored sequence values   */
static size_t terms_len = 0;          /* number of values cached  */
static size_t terms_cap = 0;          /* allocated capacity       */
static idx_t last_checked = 0;       /* largest n already tested */

static void ensure_capacity(size_t needed) {
    if (terms_cap >= needed) return;
    size_t newcap = terms_cap ? terms_cap * 2 : 16;
    if (newcap < needed) newcap = needed;
    seq_t *tmp = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
    if (!tmp) abort();      /* allocation failure; terminate */
    terms = tmp;
    terms_cap = newcap;
}

static void generate_until(idx_t target) {
    if ((idx_t)terms_len >= target) return;
    ensure_capacity(target);
    for (idx_t n = last_checked + 1; (idx_t)terms_len < target; ++n) {
        if (is_4kaprekar((seq_t)n))
            terms[terms_len++] = (seq_t)n;
    }
    last_checked = terms_len ? (idx_t)(terms[terms_len - 1]) : 0;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* offset is 1; undefined for n==0 */
    generate_until(n);
    return terms[n - 1];
}
