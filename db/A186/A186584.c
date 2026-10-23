/*
 * A186584  Number of (n+3)X7 0..2 arrays with every 4X4 subblock commuting
 *          with each horizontal and vertical neighbor 4X4 subblock.
 * https://oeis.org/A186584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 37, u64 n <= 103, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence for n > 14):
   a(1)=192026; a(2)=12263; a(3)=39757; a(4)=163723; a(5)=293665;
   a(6)=328891; a(7)=335737; a(8)=342691; a(9)=350257; a(10)=364195;
   a(11)=406729; a(12)=574195; a(13)=1025233; a(14)=1707435;
   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-4) - 4*a(n-5) + 2*a(n-6)
          + 2*a(n-8) - 2*a(n-9)   for n > 14. */

#include <stdlib.h>

/* initial 14 terms of the sequence */
static const seq_t init_vals[14] = {
    (seq_t)192026, (seq_t)12263, (seq_t)39757, (seq_t)163723,
    (seq_t)293665, (seq_t)328891, (seq_t)335737, (seq_t)342691,
    (seq_t)350257, (seq_t)364195, (seq_t)406729, (seq_t)574195,
    (seq_t)1025233, (seq_t)1707435
};

/* Sequence a(n) for n >= 1.  Cached values enable any call order. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but harmless */

    /* return directly if within the initial block */
    if (n <= 14) return init_vals[n - 1];

    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cached = 0;               /* current cache size */

    /* initial cache allocation with the base 14 values */
    if (cache == NULL) {
        cache = (seq_t *)malloc(14 * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure */
        for (idx_t i = 0; i < 14; ++i) cache[i] = init_vals[i];
        cached = 14;
    }

    /* enlarge cache and compute missing terms if needed */
    if (n > cached) {
        idx_t newsize = n;
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failure */
        cache = tmp;

        /* recurrence: a(n) = 2*a(n-1) - a(n-2) + 2*a(n-4) - 4*a(n-5)
         *                + 2*a(n-6) + 2*a(n-8) - 2*a(n-9)  (indices are 0‑based) */
        for (idx_t i = cached; i < n; ++i) {
            unsigned __int128 pos = 0;
            unsigned __int128 sub = 0;

            /* positive contributions */
            pos += (unsigned __int128)2 * (unsigned __int128)cache[i - 1];
            pos += (unsigned __int128)2 * (unsigned __int128)cache[i - 4];
            pos += (unsigned __int128)2 * (unsigned __int128)cache[i - 6];
            pos += (unsigned __int128)2 * (unsigned __int128)cache[i - 8];

            /* negative contributions */
            sub += (unsigned __int128)1 * (unsigned __int128)cache[i - 2];
            sub += (unsigned __int128)4 * (unsigned __int128)cache[i - 5];
            sub += (unsigned __int128)2 * (unsigned __int128)cache[i - 9];

            unsigned __int128 val = pos - sub;
            cache[i] = (seq_t)val;
        }
        cached = n;
    }

    return cache[n - 1];
}
