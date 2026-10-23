/*
 * A146977  a(n) = Sum_{k=1..prime(n)} binomial(2k,k).
 * https://oeis.org/A146977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..prime(n)} binomial(2k,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   alloc_seq_array, binomial, ensure_cache, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/get_prime.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/ensure_cache.h"

static size_t current_max_k = 0;
static size_t cum_cap = 0;
static seq_t *cum = NULL;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    idx_t p = get_prime(n);           /* the n‑th prime */
    size_t pk = (size_t)p;
    if (pk > current_max_k) {
        size_t new_max = pk;
        /* Ensure space for indices 0 .. new_max */
        ensure_cache((idx_t)(new_max + 1), &cum, &cum_cap);

        /* Initialize cum[0] on first run */
        if (current_max_k == 0) cum[0] = 0;

        seq_t sum = cum[current_max_k];
        for (size_t k = current_max_k + 1; k <= new_max; ++k) {
            seq_t bin = binomial((idx_t)(2 * k), (idx_t)k);
            sum = sum + bin;
            cum[k] = sum;
        }
        current_max_k = new_max;
    }
    return cum[pk];
}
