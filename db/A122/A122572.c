/*
 * A122572  a(n) = -14*a(n-1) - a(n-2), with a(1) = a(2) = 1.
 * https://oeis.org/A122572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -14*a(n-1) - a(n-2); a(1)=1, a(2)=1
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 10, i64 n <= 18, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;
static size_t seq_cache_capacity = 0;
static size_t seq_computed_up_to = 0;

static void ensure_seq_cache_capacity(size_t cap) {
    if (cap <= seq_cache_capacity) return;
    size_t new_cap = seq_cache_capacity ? seq_cache_capacity * 2 : 1;
    while (new_cap < cap) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    /* allocation is assumed to succeed; in a real setting handle errors */
    seq_cache = tmp;
    seq_cache_capacity = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* out of spec, but safe guard */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;

    size_t target = (size_t)n;
    ensure_seq_cache_capacity(target);

    /* initialize the first two terms on first use */
    if (seq_computed_up_to == 0) {
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)1;
        seq_computed_up_to = 2;
    }

    if (target > seq_computed_up_to) {
        for (size_t i = seq_computed_up_to; i < target; ++i) {
            /* i is zero–based index for the term number i+1 */
            seq_cache[i] = - (seq_t)14 * seq_cache[i - 1] - seq_cache[i - 2];
        }
        seq_computed_up_to = target;
    }
    return seq_cache[n - 1];
}
