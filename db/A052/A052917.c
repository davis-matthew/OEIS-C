/*
 * A052917  Expansion of 1/(1-3*x-x^4).
 * https://oeis.org/A052917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-4), a(0) = 1, a(1) = 3, a(2) = 9, a(3) =
 *            27
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/grow_seq_array.h"

seq_t a(idx_t n) {
    static bool initialized = false;
    static seq_t *cache = NULL;    /* array of sequence values */
    static size_t cap   = 0;       /* allocated capacity */
    static size_t up_to = 3;       /* highest index whose value is already known */

    /* initial allocation and base values */
    if (!initialized) {
        size_t min_cap = 4;
        if ((size_t)n + 1 > min_cap) min_cap = (size_t)n + 1;
        grow_seq_array(&cache, &cap, min_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)27;
        initialized = true;
    }

    /* ensure the buffer can hold a(n) */
    if ((size_t)n + 1 > cap) {
        grow_seq_array(&cache, &cap, (size_t)n + 1);
    }

    /* for n < 4 the value is already stored */
    if ((size_t)n < 4) {
        return cache[(size_t)n];
    }

    /* extend the computed sequence up to n */
    if (up_to < (size_t)n) {
        for (size_t i = up_to + 1; i <= (size_t)n; ++i) {
            cache[i] = cache[i - 1] * (seq_t)3 + cache[i - 4];
        }
        up_to = (size_t)n;
    }

    return cache[(size_t)n];
}
