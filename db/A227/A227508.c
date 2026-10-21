/*
 * A227508  Numbers n such that n! is divisible by R(n), the digit reversal of
 *          n.
 * https://oeis.org/A227508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   rev_divides_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A227508: numbers n whose decimal reversal R(n) divides n!.
   This implementation searches successive integers, keeps a cache of
   already found terms, and returns the n‑th term (offset 1). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/rev_divides_factorial.h"

/* Return the n-th term (offset 1) of A227508. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* undefined offset */

    /* Static cache of previously found terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0, found = 0;
    static idx_t current = 1;

    while (found < n) {
        if (rev_divides_factorial(current)) {
            if (found >= cache_size) {
                size_t new_cap = cache_size ? cache_size << 1 : 16;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_size = (idx_t)new_cap;
            }
            cache[found++] = (seq_t)current;
        }
        ++current;
    }
    return cache[n - 1];
}
