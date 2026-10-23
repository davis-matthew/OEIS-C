/*
 * A178713  Numbers k such that the sum of the subfactorial of the decimal
 *          digits of k is a square.
 * https://oeis.org/A178713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A178713: a(n) is the n‑th integer k such that the sum of
   subfactorials of the decimal digits of k is a perfect square. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static const seq_t SUBFACT[10] = {
    (seq_t)1,   /* !0 */
    (seq_t)0,   /* !1 */
    (seq_t)1,   /* !2 */
    (seq_t)2,   /* !3 */
    (seq_t)9,   /* !4 */
    (seq_t)44,  /* !5 */
    (seq_t)265, /* !6 */
    (seq_t)1854,/* !7 */
    (seq_t)14833,/* !8 */
    (seq_t)133496/* !9 */
};

static seq_t *terms = NULL;
static size_t terms_len = 0, terms_cap = 0;
static uint64_t next_k = 0;

static void append_term(seq_t val)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 32;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

static bool is_sum_square(uint64_t k)
{
    seq_t sum = (seq_t)0;
    if (k == 0) {
        sum = SUBFACT[0];
    } else {
        while (k > 0) {
            sum += SUBFACT[k % 10];
            k /= 10;
        }
    }
    seq_t sq = floor_sqrt(sum);
    return sq * sq == sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1 */

    while (terms_len < n) {
        uint64_t k = next_k++;
        if (is_sum_square(k))
            append_term((seq_t)k);
    }
    return terms[n - 1];
}
