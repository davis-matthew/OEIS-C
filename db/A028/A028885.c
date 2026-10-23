/*
 * A028885  Numbers k such that k^2 - 8 is prime.
 * https://oeis.org/A028885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Numbers k such that k^2 - 8 is prime a(n) is the n-th such k
 *            (offset 1)
 * Range:     u8 n <= 4, u16 n <= 54, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t count = 0;
    static idx_t next_k = 5;          /* starting candidate */

    while (count < n) {
        /* Grow storage if needed */
        if (count + 1 > cap) {
            ensure_capacity(count + 1, &cap, (void **)&cache, sizeof(seq_t));
            if (!cache) return 0;    /* allocation failed */
        }

        seq_t k = (seq_t)next_k;
        seq_t candidate = k * k - (seq_t)8;
        if (is_prime((idx_t)candidate)) {
            cache[count++] = k;
        }
        next_k++;
    }
    return cache[n - 1];
}
