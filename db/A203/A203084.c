/*
 * A203084  Number of (n+2) X 3 binary arrays avoiding patterns 000 and 101 in
 *          rows and columns.
 * https://oeis.org/A203084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+6*a(n-3)+2*a(n-4)+3*a(n-5) - a(n-6) -
 *            a(n-8) - a(n-9) - a(n-10) for n ≥ 11; Initial values: a(1)=98,
 *            a(2)=253, a(3)=621, a(4)=1575, a(5)=4000, a(6)=10057,
 *            a(7)=25384, a(8)=64140, a(9)=161819, a(10)=408415
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 20, u64 n <= 43, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base[11] = {
    0ULL,          /* unused placeholder for index 0 */
    98ULL, 253ULL, 621ULL, 1575ULL, 4000ULL,
    10057ULL, 25384ULL, 64140ULL, 161819ULL, 408415ULL
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety for out of bounds */

    /* static memoisation state */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;         /* allocated array size */
    static idx_t   last  = 0;         /* highest index computed */

    /* ensure allocated storage is large enough (at least up to n) */
    size_t required = (size_t)n + 1;   /* indices 0..n inclusive */
    if (required < 11) required = 11;  /* make room for initial 10 terms */
    if (cap < required) {
        seq_t *tmp = (seq_t *)realloc(cache, required * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);  /* allocation failed */
        cache = tmp;
        cap   = required;
    }

    /* initialise the cache with the first 10 terms exactly once */
    if (last == 0) {
        for (size_t i = 1; i <= 10; ++i)
            cache[i] = base[i];
        last = 10;          /* we now have values for indices 1..10 */
    }

    /* if the requested n is already computed, simply return it */
    if (n <= last) return cache[(size_t)n];

    /* compute subsequent terms using the recurrence (n ≥ 11) */
    for (idx_t i = last + 1; i <= n; ++i) {
        size_t k = (size_t)i;      /* index in the array */
        seq_t val =
            cache[k-1] + cache[k-2] + 6 * cache[k-3] +
            2 * cache[k-4] + 3 * cache[k-5];
        val -= cache[k-6];
        val -= cache[k-8];
        val -= cache[k-9];
        val -= cache[k-10];
        cache[k] = val;
    }
    last = n;
    return cache[(size_t)n];
}
