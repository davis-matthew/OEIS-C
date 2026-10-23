/*
 * A108938  Numbers k such that 911*k + 7 is prime.
 * https://oeis.org/A108938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A108938: numbers k such that 911*k + 7 is prime.
   Return the n-th (1‑based) such k. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Static state for memoization and incremental search. */
    static seq_t *k_vals = NULL;   /* array of found k values          */
    static idx_t k_cap  = 0;       /* current capacity of k_vals       */
    static idx_t k_size = 0;       /* number of stored values          */
    static idx_t next_k = 0;       /* next candidate k to test         */

    /* Expand the table until we have at least n values. */
    while (k_size < n) {
        uint64_t num = (uint64_t)911ULL * next_k + 7ULL; /* 911*k + 7 */

        if (is_prime(num)) {
            /* Ensure enough space for a new entry. */
            if (k_size == k_cap) {
                idx_t new_cap = k_cap == 0 ? 128 : k_cap << 1;
                seq_t *tmp = (seq_t *)realloc(k_vals, new_cap * sizeof(seq_t));
                if (!tmp) abort();            /* allocation failure */
                k_vals = tmp;
                k_cap  = new_cap;
            }
            k_vals[k_size++] = (seq_t)next_k;   /* store the k value   */
        }

        ++next_k;
    }

    return k_vals[n - 1];
}
