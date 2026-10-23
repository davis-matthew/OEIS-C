/*
 * A195556  Denominators a(n) of Pythagorean approximations b(n)/a(n) to 1/3.
 * https://oeis.org/A195556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 37*a(n-3)+37*a(n-6)-a(n-9) (n ≥ 10)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*        a(1) =  1, a(2) = 12, a(3) = 24, a(4) = 35, a(5) = 468, */
/*       a(6) =  900, a(7) = 1333, a(8) = 17760, a(9) = 34188    */

#include <stdlib.h>   /* malloc, realloc */

extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* The sequence is 1‑indexed.  Nothing is required for n < 1. */
    if (n < 1) return (seq_t)0;

    static seq_t *cache = NULL;      /* circular buffer of previously
                                         computed terms                     */
    static idx_t cache_len = 0;      /* number of terms stored           */

    /* ensure cache large enough for term n */
    if (n > cache_len) {
        idx_t new_len = n;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)new_len);
        if (!new_cache) return (seq_t)0;   /* allocation failed */

        /* initialise new entries */
        static const seq_t base[9] = {
            (seq_t)1,      /* a(1) */
            (seq_t)12,     /* a(2) */
            (seq_t)24,     /* a(3) */
            (seq_t)35,     /* a(4) */
            (seq_t)468,    /* a(5) */
            (seq_t)900,    /* a(6) */
            (seq_t)1333,   /* a(7) */
            (seq_t)17760,  /* a(8) */
            (seq_t)34188   /* a(9) */
        };

        for (idx_t i = cache_len; i < new_len; ++i) {
            idx_t idx = i + 1; /* term number */

            if (idx <= 9) {
                new_cache[i] = base[i];
            } else {
                /* recurrence: a(n) = 37*a(n-3) + 37*a(n-6) - a(n-9) */
                seq_t t1 = new_cache[i - 3];
                seq_t t2 = new_cache[i - 6];
                seq_t t3 = new_cache[i - 9];

                seq_t r = (seq_t)37 * t1;
                r = r + (seq_t)37 * t2;
                r = r - t3;

                new_cache[i] = r;
            }
        }

        cache = new_cache;
        cache_len = new_len;
    }

    return cache[n - 1];
}
