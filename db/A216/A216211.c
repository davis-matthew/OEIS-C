/*
 * A216211  Number of self-avoiding walks of any length from NW to SW corners
 *          of a grid or lattice with n rows and 4 columns.
 * https://oeis.org/A216211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = 12*a(n-1) - 54*a(n-2) + 124*a(n-3) - 133*a(n-4) -
 *            16*a(n-5) + 175*a(n-6) - 94*a(n-7) - 69*a(n-8) + 40*a(n-9) +
 *            12*a(n-10) - 4*a(n-11) - a(n-12) for n>12; Initial values:
 *            a(1)=1, a(2)=4, a(3)=28, a(4)=178, a(5)=1008, a(6)=5493,
 *            a(7)=29879, a(8)=163357, a(9)=895519, a(10)=4911542,
 *            a(11)=2693...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_a12[12] = {
    (seq_t)1, (seq_t)4, (seq_t)28, (seq_t)178, (seq_t)1008,
    (seq_t)5493, (seq_t)29879, (seq_t)163357, (seq_t)895519,
    (seq_t)4911542, (seq_t)26932856, (seq_t)147666219
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of range, not used */
    if (n <= 12) return init_a12[n - 1];

    static seq_t *cache = NULL;
    static idx_t max_computed = 0;        /* highest n with a value stored */

    /* initialize cache on first call */
    if (cache == NULL) {
        /* allocate space for indices 0..12 (index 0 unused) */
        cache = (seq_t *)malloc((12 + 1) * sizeof(seq_t));
        if (cache == NULL) abort();      /* out of memory */
        cache[0] = (seq_t)0;
        for (idx_t i = 1; i <= 12; ++i) cache[i] = init_a12[i - 1];
        max_computed = 12;
    }

    /* grow cache if necessary */
    if (n > max_computed) {
        size_t new_cap = (size_t)n + 1;   /* indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();         /* out of memory */
        cache = tmp;
    }

    /* compute missing values only */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t val = (seq_t)12 * cache[i - 1];
        val -= (seq_t)54 * cache[i - 2];
        val += (seq_t)124 * cache[i - 3];
        val -= (seq_t)133 * cache[i - 4];
        val -= (seq_t)16  * cache[i - 5];
        val += (seq_t)175 * cache[i - 6];
        val -= (seq_t)94  * cache[i - 7];
        val -= (seq_t)69  * cache[i - 8];
        val += (seq_t)40  * cache[i - 9];
        val += (seq_t)12  * cache[i -10];
        val -= (seq_t)4   * cache[i -11];
        val -= cache[i -12];
        cache[i] = val;
    }
    max_computed = n;
    return cache[n];
}
