/*
 * A063425  Unattainable numbers: integers not expressible as k + product of
 *          nonzero digits of k (A063114).
 * https://oeis.org/A063425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A063425: integers not representable as k + product_of_nonzero_digits(k). */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* Build a table of reachable numbers up to a given limit
   and collect all unattainable numbers up to that limit. */
static void build_table(idx_t limit, seq_t **unatt_ptr, size_t *cnt_ptr) {
    /* reachable[n] == true  <=>  n is expressible as k + prod(k) */
    bool *reachable = (bool*)calloc((size_t)(limit + 1), sizeof(bool));
    if (!reachable) return; /* allocation failure – should not happen in tests */

    for (idx_t k = 1; k <= limit; ++k) {
        uint64_t prod = 1;
        idx_t x = k;
        while (x > 0) {
            unsigned d = (unsigned)(x % 10);
            if (d != 0) prod *= d;
            x /= 10;
        }
        idx_t m = k + prod;
        if (m <= limit) reachable[(size_t)m] = true;
    }

    size_t cnt = 0;
    for (idx_t n = 1; n <= limit; ++n) {
        if (!reachable[(size_t)n]) cnt++;
    }

    seq_t *unatt = seq_alloc_new_array(cnt);
    if (!unatt) {
        free(reachable);
        return; /* allocation failure – should not happen in tests */
    }
    size_t pos = 0;
    for (idx_t n = 1; n <= limit; ++n) {
        if (!reachable[(size_t)n]) {
            unatt[pos++] = (seq_t)n;
        }
    }

    free(reachable);
    *unatt_ptr = unatt;
    *cnt_ptr   = cnt;
}

/* The OEIS function a(n). */
seq_t a(idx_t n) {
    static bool initialized = false;
    static seq_t *unatt     = NULL;
    static size_t unatt_cnt = 0;
    static idx_t cur_limit  = 0;

    if (!initialized) {
        idx_t limit = 5000000;                /* initial limit – covers n ≈ 10000 */
        build_table(limit, &unatt, &unatt_cnt);
        cur_limit = limit;
        initialized = true;
    }

    /* Extend the table if necessary. */
    while (n > unatt_cnt) {
        idx_t new_limit = cur_limit * 2;
        seq_t *new_unatt = NULL;
        size_t new_cnt   = 0;
        build_table(new_limit, &new_unatt, &new_cnt);

        seq_free_array(unatt);
        unatt     = new_unatt;
        unatt_cnt = new_cnt;
        cur_limit = new_limit;
    }

    return unatt[n - 1];      /* a(1) is the first element, so index n-1 */
}
