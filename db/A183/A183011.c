/*
 * A183011  (24n - 1)p(n): traces of partition class polynomials, with a(0) =
 *          -1.
 * https://oeis.org/A183011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (24*n - 1) * p(n) for n >= 1, a(0) = -1 where p(n) is the
 *            partition number. The partition numbers are computed via the
 *            generalized pentagonal theorem. The function stores a static
 *            table of partitions and grows it as needed
 * Range:     i8 n <= 2, i16 n <= 13, i32 n <= 62, i64 n <= 273,
 *            i128 n <= 1145, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Partition cache */
static seq_t *p_cache = NULL;
static size_t p_cap = 0;

/* Ensure that p_cache has entries up to and including n. */
static void ensure_partitions(idx_t n) {
    if ((size_t)n < p_cap) return;           /* already computed */

    /* Grow capacity: double until it exceeds n. */
    size_t old_cap = p_cap;
    size_t new_cap = (p_cap == 0) ? 1 : p_cap;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    seq_t *new_ptr = (seq_t *)realloc(p_cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();
    p_cache = new_ptr;
    p_cap = new_cap;

    /* Compute new entries. */
    for (size_t i = old_cap; i < new_cap; ++i) {
        if (i == 0) {
            p_cache[i] = (seq_t)1;        /* p(0) = 1 */
            continue;
        }
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent1 > i && pent2 > i) break;
            seq_t sign = (k % 2 ? (seq_t)1 : (seq_t)-1);
            if (pent1 <= i) sum += sign * p_cache[i - pent1];
            if (pent2 <= i) sum += sign * p_cache[i - pent2];
        }
        p_cache[i] = sum;
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)-1;

    ensure_partitions(n);

    seq_t pn = p_cache[n];
    seq_t mult = ((seq_t)n * 24) - 1;
    return mult * pn;
}
