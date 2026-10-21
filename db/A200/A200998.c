/*
 * A200998  Triangular numbers, T(m), that are three-quarters of another
 *          triangular number: T(m) such that 4*T(m)=3*T(k) for some k.
 * https://oeis.org/A200998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 21, a(n) = 194*a(n-1) - a(n-2) + 21 for n >= 2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

/* Maximum n expected for this implementation (≈10000) */
#define MAX_CACHE 10001

seq_t a(idx_t n)
{
    static seq_t cache[MAX_CACHE];
    static size_t computed = 0;          /* number of terms already stored */

    size_t idx = (size_t)n;
    if (idx < computed)                  /* value already computed */
        return cache[idx];

    for (size_t i = computed; i <= idx; ++i) {
        if (i == 0)
            cache[i] = (seq_t)0;
        else if (i == 1)
            cache[i] = (seq_t)21;
        else
            cache[i] = (seq_t)194 * cache[i - 1] - cache[i - 2] + (seq_t)21;
    }
    computed = idx + 1;
    return cache[idx];
}
