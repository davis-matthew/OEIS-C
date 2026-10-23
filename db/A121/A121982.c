/*
 * A121982  Numbers k such that k^2 + 15 is prime.
 * https://oeis.org/A121982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 35, u32 n <= 4511, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   grow_seq, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that k^2 + 15 is prime.
   The function constructs the sequence incrementally and caches results.
   It uses incremental prime testing via isPrimeGeneric. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n) {
    /* n is 1‑based.  If n is zero, return zero (should not happen). */
    if (n == 0) return 0;

    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static idx_t next_k = 1; /* Next candidate k to test */

    /* Generate terms until we have at least n of them. */
    while (len < n) {
        idx_t k = next_k++;
        seq_t ksq = (seq_t)k * (seq_t)k;
        seq_t val = ksq + (seq_t)15;

        if (isPrimeGeneric(val)) {
            /* grow the cache if necessary */
            grow_seq(&cache, &cap, len + 1);
            cache[len++] = (seq_t)k; /* store k, not k^2+15 */
        }
    }

    return cache[n - 1];
}
