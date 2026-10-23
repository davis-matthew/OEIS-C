/*
 * A178460  Partial sums of floor(2^n/127).
 * https://oeis.org/A178460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = a(n‑7) + 2^(n‑6) – 1 for n > 6, with a(1)…a(6) = 0. This
 *            follows from the definition a(n) = Σ_{k=1..n} ⌊2^k/127⌋
 * Range:     u8 n <= 13, u16 n <= 21, u32 n <= 37, u64 n <= 69, u128 n <= 133,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Static cache for memoised values.  cache[0] is unused for the
       sequence itself but stores 0 to simplify the recurrence. */
    static seq_t *cache = NULL;
    static idx_t capacity = 0;   /* total allocated entries (>= n + 1) */
    static idx_t last    = 0;    /* highest index already computed */

    if (n == 0) return (seq_t)0;   /* defensive */

    /* Grow the cache if necessary. */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = capacity?capacity:16;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;        /* double */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) abort();                               /* allocation failure */
        capacity = (idx_t)new_cap;
    }

    /* Initialise the zero entry if the array was just created. */
    if (last == 0 && capacity > 0) cache[0] = (seq_t)0;

    /* Compute any values that have not yet been cached up to n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 6) {
            val = (seq_t)0;
        } else {
            val = cache[i - 7];        /* a(i-7) */
            val += pow2(i - 6);       /* 2^(i-6) */
            val -= (seq_t)1;
        }
        cache[i] = val;
    }
    last = n;
    return cache[n];
}
