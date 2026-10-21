/*
 * A240806  Coefficients in expansion of graph zeta function of graph obtained
 *          by adding 4 vertices to each edge of K_5.
 * https://oeis.org/A240806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) - 6*a(n-3) - 9*a(n-4) - 15*a(n-5) +
 *            35*a(n-6) + 60*a(n-7) - 75*a(n-8) - 75*a(n-9) + 81*a(n-10) +
 *            42*a(n-11) - 43*a(n-12) - 9*a(n-13) + 9*a(n-14) for n > 13;
 *            a(0)=1, a(1)=3, a(2)=12, a(3)=39, a(4)=126, a(5)=381, a(6)=1169,
 *            a(7)=3528, a(8)=10611, a(9)=31869, a(10)=95742,...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for realloc, exit, EXIT_FAILURE */

static const seq_t init_vals[15] = {
    (seq_t)1,        /* a(0) */
    (seq_t)3,        /* a(1) */
    (seq_t)12,       /* a(2) */
    (seq_t)39,       /* a(3) */
    (seq_t)126,      /* a(4) */
    (seq_t)381,      /* a(5) */
    (seq_t)1169,     /* a(6) */
    (seq_t)3528,     /* a(7) */
    (seq_t)10611,    /* a(8) */
    (seq_t)31869,    /* a(9) */
    (seq_t)95742,    /* a(10) */
    (seq_t)287235,   /* a(11) */
    (seq_t)861753,   /* a(12) */
    (seq_t)2585646,  /* a(13) */
    (seq_t)7757199   /* a(14) */
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array of computed terms */
    static idx_t cache_len = 0;      /* current allocation size */
    static idx_t computed = 0;       /* number of terms already initialized */

    /* Grow allocation if necessary */
    if (n + 1 > cache_len) {
        seq_t *tmp = (seq_t*)realloc(cache,
                                     (n + 1) * sizeof(seq_t));
        if (!tmp) {
            exit(EXIT_FAILURE);   /* allocation failed */
        }
        cache = tmp;
        cache_len = n + 1;
    }

    /* Compute missing terms up to n */
    if (n + 1 > computed) {
        for (idx_t i = computed; i <= n; ++i) {
            if (i < 15) {
                cache[i] = init_vals[i];
            } else {
                seq_t v = 3 * cache[i-1]
                        + 3 * cache[i-2]
                        - 6 * cache[i-3]
                        - 9 * cache[i-4]
                        - 15 * cache[i-5]
                        + 35 * cache[i-6]
                        + 60 * cache[i-7]
                        - 75 * cache[i-8]
                        - 75 * cache[i-9]
                        + 81 * cache[i-10]
                        + 42 * cache[i-11]
                        - 43 * cache[i-12]
                        - 9 * cache[i-13]
                        + 9 * cache[i-14];
                cache[i] = v;
            }
        }
        computed = n + 1;
    }

    return cache[n];
}
