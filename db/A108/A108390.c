/*
 * A108390  Expansion of (-1-x-x^2-4*x^3-4*x^4+4*x^5+x^6+x^7+x^8) /
 *          ((x+1)*(x^2-x+1)*(x^2+x-1)*(x^4-x^3+2*x^2+x+1)).
 * https://oeis.org/A108390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*a(n-3) + 5*a(n-6) + a(n-9) (n > 8) * Initial values (n
 *            = 0 .. 8): * a(0) = 1, a(1) = 1, a(2) = 1, a(3) = 7, a(4) = 7, *
 *            a(5) = -1, a(6) = 25, a(7) = 25, a(8) = 1
 * Range:     i8 n <= 11, i16 n <= 20, i32 n <= 44, i64 n <= 89, i128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache      = NULL;
    static idx_t  capacity   = 0;
    static idx_t  size_filled = 0;  /* number of terms already stored */

    /* base values for indices 0..8 */
    static const seq_t base[9] = { 1, 1, 1, 7, 7, -1, 25, 25, 1 };

    /* first call: allocate cache and store base values */
    if (cache == NULL) {
        capacity = 64;                     /* arbitrary start size */
        cache    = (seq_t *)malloc(capacity * sizeof(seq_t));
        for (idx_t i = 0; i < 9; ++i) cache[i] = base[i];
        size_filled = 9;
    }

    /* return early if the result is already known */
    if (n < size_filled) return cache[n];

    /* enlarge cache if necessary */
    if (n >= capacity) {
        idx_t new_cap = capacity;
        while (new_cap <= n) new_cap <<= 1;   /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failed – unlikely in these tests */
        cache    = tmp;
        capacity = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = size_filled; i <= n; ++i) {
        seq_t val = 3 * cache[i - 3] + 5 * cache[i - 6] + cache[i - 9];
        cache[i] = val;
    }
    size_filled = n + 1;

    return cache[n];
}
