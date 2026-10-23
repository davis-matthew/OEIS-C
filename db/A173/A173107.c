/*
 * A173107  Partial sums of abundant numbers (A005101).
 * https://oeis.org/A173107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum of the first n abundant numbers (A005101)
 * Range:     u8 n <= 8, u16 n <= 176, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static idx_t current_limit = 0;
static size_t abund_count = 0;
static seq_t *prefix = NULL;

/* Build abundant‐number prefix sums up to the given limit. */
static void recompute_abundants(idx_t limit) {
    size_t sz = (size_t)limit + 1;
    uint64_t *sigma = (uint64_t *)malloc(sz * sizeof(uint64_t));
    if (!sigma) exit(1);
    for (size_t i = 0; i < sz; ++i) sigma[i] = 0;
    for (size_t i = 1; i < sz; ++i)
        for (size_t j = i; j < sz; j += i)
            sigma[j] += i;

    size_t count = 0;
    for (size_t i = 1; i < sz; ++i)
        if (sigma[i] > ((uint64_t)i << 1)) ++count;

    seq_t *new_prefix = alloc_seq_array(count);
    seq_t sum = 0;
    size_t idx = 0;
    for (size_t i = 1; i < sz; ++i) {
        if (sigma[i] > ((uint64_t)i << 1)) {
            sum += (seq_t)i;
            new_prefix[idx++] = sum;
        }
    }
    free(sigma);
    free_seq_array(prefix);
    prefix = new_prefix;
    abund_count = count;
    current_limit = limit;
}

/* Ensure that at least "required" abundant numbers are cached. */
static void ensure_abundants(idx_t required) {
    if (required <= abund_count) return;

    idx_t limit = current_limit > 0 ? current_limit : 1000;
    while (abund_count < required) {
        if (limit < required * 5) limit = required * 5;
        else                       limit *= 2;
        recompute_abundants(limit);
    }
}

/* External interface required by the harness. */
seq_t a(idx_t n) {
    if (n == 0) return 0;               /* undefined for n<1, but harmless */
    ensure_abundants(n);
    return prefix[n - 1];
}
