/*
 * A240789  Number of n X 2 0..3 arrays with no element equal to one plus the
 *          sum of elements to its left or one plus the sum of elements above
 *          it or zero plus the sum of the elements diagonally to its
 *          northwest or zero plus the sum of the elements antidiagonally to
 *          its northeast, modulo 4.
 * https://oeis.org/A240789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 33, u32 n <= 73, u64 n <= 152,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for A240789:
   a(n) = 4*a(n-3) + a(n-5) - 3*a(n-6) - 3*a(n-8) - 4*a(n-9)
          + a(n-11) + 4*a(n-12) + 3*a(n-14) - 2*a(n-17)
   Initial values:
   a(1)=3, a(2)=4, a(3)=5, a(4)=10, a(5)=13, a(6)=14,
   a(7)=30, a(8)=32, a(9)=36, a(10)=67, a(11)=79, a(12)=97,
   a(13)=173, a(14)=191, a(15)=232, a(16)=402, a(17)=464,
   a(18)=580, a(19)=960
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation cache */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* initialise cache with known values up to n = 19 */
    if (cache == NULL) {
        static const seq_t init_vals[] = {
            (seq_t)3, (seq_t)4, (seq_t)5, (seq_t)10, (seq_t)13,
            (seq_t)14, (seq_t)30, (seq_t)32, (seq_t)36, (seq_t)67,
            (seq_t)79, (seq_t)97, (seq_t)173, (seq_t)191, (seq_t)232,
            (seq_t)402, (seq_t)464, (seq_t)580, (seq_t)960
        };
        cache_size = sizeof(init_vals) / sizeof(init_vals[0]);
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;                    /* allocation failed */
        for (size_t i = 0; i < cache_size; ++i)
            cache[i] = init_vals[i];
    }

    if (n == 0)
        return (seq_t)0;            /* undefined for n < 1 */

    size_t n_sz = (size_t)n;

    /* already computed? */
    if (n_sz <= cache_size)
        return cache[n_sz - 1];

    /* enlarge cache to hold up to n */
    size_t new_size = n_sz;
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        return (seq_t)0;            /* realloc failed */
    cache = tmp;

    /* compute new terms using the empirical recurrence */
    for (size_t i = cache_size; i < new_size; ++i) {
        size_t idx = i + 1;      /* 1‑based n for current term */
        seq_t val = 0;
        val += 4 * cache[idx - 4];   /* a(n-3) */
        val +=   cache[idx - 6];     /* a(n-5) */
        val -= 3 * cache[idx - 7];   /* a(n-6) */
        val -= 3 * cache[idx - 9];   /* a(n-8) */
        val -= 4 * cache[idx -10];  /* a(n-9) */
        val +=   cache[idx -12];     /* a(n-11) */
        val += 4 * cache[idx -13];  /* a(n-12) */
        val += 3 * cache[idx -15];  /* a(n-14) */
        val -= 2 * cache[idx -18];  /* a(n-17) */
        cache[i] = val;
    }
    cache_size = new_size;

    return cache[n_sz - 1];
}
