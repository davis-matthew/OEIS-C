/*
 * A227225  Numbers k such that (sum of digits of k) + (product of digits of
 *          k) is a perfect cube.
 * https://oeis.org/A227225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   digit_product, digit_sum, floor_cube_root, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The nth term of A227225 is the nth number k (starting from 0) for which
 *   sum_of_digits(k) + product_of_digits(k)
 * is a perfect cube.  The function expands the sequence lazily and
 * memoises all found values.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/digit_sum.h"
#include "../utils/digit_product.h"
#include "../utils/floor_cube_root.h"
#include "../utils/seq_mul.h"

static seq_t *terms = NULL;        /* memoised sequence values */
static size_t term_count = 0;      /* number of values currently stored */
static size_t term_cap   = 0;      /* storage capacity */

/* Ensure capacity for at least <needed> elements. */
static void grow_terms(size_t needed)
{
    if (term_cap >= needed) return;
    size_t new_cap = term_cap ? term_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();          /* out of memory */
    terms = new_ptr;
    term_cap = new_cap;
}

/* Extend the memoised sequence until it contains at least <n> terms. */
static void extend_until(idx_t n)
{
    if (term_count >= n) return;
    grow_terms(n);

    /* Start searching from the next integer after the last stored term. */
    idx_t cand = term_count
                 ? ((idx_t)terms[term_count - 1]) + 1ULL
                 : 0ULL;

    while (term_count < n) {
        seq_t cand_seq = (seq_t)cand;

        seq_t s = digit_sum(cand_seq);
        seq_t p = digit_product(cand_seq);
        seq_t v = s + p;

        seq_t r = floor_cube_root(v);
        seq_t r3 = seq_mul(seq_mul(r, r), r);

        if (r3 == v) {
            terms[term_count++] = cand_seq;
        }
        ++cand;
    }
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; protects against n=0 */
    extend_until(n);
    return terms[n - 1];
}
