/*
 * A107287  a(n) = a(n-1)+a(n-2)-a(n-3)+a(n-5), n>7.
 * https://oeis.org/A107287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)-a(n-3)+a(n-5) for n ≥ 8 * Initial values: *
 *            a(0)=1, a(1)=0, a(2)=1, a(3)=1, a(4)=1, * a(5)=4, a(6)=4, a(7)=6
 * Range:     u8 n <= 17, u16 n <= 32, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* 20000 is comfortably larger than the maximum n (≈10000) we
     * expect in the tests; the array resides in static storage, so
     * no dynamic allocation (and no constructor issues for
     * big‑integer types). */
    static seq_t cache[20000];
    static size_t cached_size = 0;   /* number of elements that are valid */

    /* Initialise the cache once with the first eight terms. */
    if (cached_size == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)1;
        cache[5] = (seq_t)4;
        cache[6] = (seq_t)4;
        cache[7] = (seq_t)6;
        cached_size = 8;   /* indices 0..7 are now stored */
    }

    /* Return if already computed. */
    size_t idx = (size_t)n;
    if (idx < cached_size) {
        return cache[idx];
    }

    /* Generate terms up to idx using the recurrence. */
    while (cached_size <= idx) {
        seq_t val = cache[cached_size - 1];
        val += cache[cached_size - 2];
        val -= cache[cached_size - 3];
        val += cache[cached_size - 5];
        cache[cached_size] = val;
        ++cached_size;
    }

    return cache[idx];
}
