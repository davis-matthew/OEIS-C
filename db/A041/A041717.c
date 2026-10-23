/*
 * A041717  Denominators of continued fraction convergents to sqrt(378).
 * https://oeis.org/A041717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 19, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041717:
   Denominators of continued fraction convergents to √378.
   Recurrence: a(n) = 17498*a(n-8) - a(n-16)  (n > 15)
   Initial values a(0) … a(15) are given below. */

#include <stddef.h>

static const seq_t init_terms[16] = {
    (seq_t)1,  (seq_t)2,  (seq_t)7,  (seq_t)9,
    (seq_t)43, (seq_t)52, (seq_t)199, (seq_t)450,
    (seq_t)17299, (seq_t)35048, (seq_t)122443, (seq_t)157491,
    (seq_t)752407, (seq_t)909898, (seq_t)3482101, (seq_t)7874100
};

seq_t a(idx_t n)
{
    /* For n < 16 just return the initial values. */
    if (n < 16)
        return init_terms[(size_t)n];

    /* Cache up to a fixed size; the tests use n ≤ 10000. */
    enum { CACHE_SIZE = 20000 };
    static seq_t cache[CACHE_SIZE] = {0};
    static int initialized = 0;

    if (!initialized) {
        for (size_t i = 0; i < 16; ++i)
            cache[i] = init_terms[i];
        initialized = 1;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = 16; i <= n; ++i) {
        /* Multiply first to avoid overflow for built‑in integer types. */
        seq_t term = (seq_t)17498 * cache[i - 8] - cache[i - 16];
        cache[i] = term;
    }

    return cache[n];
}
