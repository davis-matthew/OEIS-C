/*
 * A108980  Numbers k such that the string k9111 is prime.
 * https://oeis.org/A108980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer k for which 10000*k + 9111 is prime
 * Range:     u8 n <= 43, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes terms by searching successive candidates and
   testing primality of the corresponding 5‑digit-affixed integer. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *_cache = NULL;   /* cache of previously found k values       */
static idx_t  _cnt   = 0;       /* number of cached values (same as a(i))   */
static idx_t  _cap   = 0;       /* current allocated capacity              */

/* Ensure the cache has space for at least 'min_cap' elements. */
static void ensure_capacity(idx_t min_cap) {
    if (_cap < min_cap) {
        idx_t new_cap = _cap ? _cap * 2 : 64;
        if (new_cap < min_cap) new_cap = min_cap;
        seq_t *new_arr = (seq_t *)realloc(_cache, (size_t)new_cap * sizeof(seq_t));
        if (!new_arr) exit(EXIT_FAILURE);   /* memory allocation failed */
        _cache = new_arr;
        _cap   = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;   /* sequence defined for n ≥ 1; 0 is a safe default */

    /* Expand the cache until it contains at least 'n' elements. */
    while (_cnt < n) {
        seq_t k;   /* candidate value for k */

        if (_cnt == 0) {
            k = 1;   /* start testing from k = 1 */
        } else {
            k = _cache[_cnt - 1] + 1;   /* next integer after last found k */
        }

        /* Find the next k such that 10000*k + 9111 is prime. */
        while (true) {
            uint64_t test_num = (uint64_t)10000 * (uint64_t)k + 9111ULL;
            if (is_prime(test_num)) {
                /* Store the found k, ensuring capacity. */
                ensure_capacity(_cnt + 1);
                _cache[_cnt] = k;
                _cnt++;
                break;
            }
            k++;
        }
    }

    return _cache[n - 1];
}
