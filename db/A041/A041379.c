/*
 * A041379  Denominators of continued fraction convergents to sqrt(204).
 * https://oeis.org/A041379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A041379 – denominators of continued‑fraction convergents to sqrt(204)
 *
 *  Formula (recurrence):
 *      a(n) = 9998*a(n-8) - a(n-16)   for n > 15
 *
 *  Initial values (n = 0 … 15):
 *      1, 3, 4, 7, 46, 53, 99, 350,
 *      9899, 30047, 39946, 69993, 459904, 529897, 989801, 3499300
 */

#include <stdlib.h>

/*  seq_t  and  idx_t  are supplied by the build environment.
     Do NOT define them here.  */

seq_t a(idx_t n)
{
    /* static cache for memoisation – fits both C and C++                    */
    static seq_t *cache      = NULL;   /* pointer to array of seq_t          */
    static size_t cache_len  = 0;      /* number of valid entries in cache   */

    /* extend the cache so that cache[n] is available                    */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp;

#ifdef __cplusplus
        /* C++: need an explicit cast of the void* returned by realloc() */
        tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
#else
        /* C: implicit conversion from void* to the required type is OK */
        tmp = realloc(cache, new_len * sizeof(seq_t));
#endif
        if (!tmp) return 0;  /* allocation failed – give a safe default */

        cache = tmp;

        /*  initial 16 values – explicitly promote constants to seq_t   */
        static const seq_t init[16] = {
            (seq_t)1,   (seq_t)3,   (seq_t)4,   (seq_t)7,
            (seq_t)46,  (seq_t)53,  (seq_t)99,  (seq_t)350,
            (seq_t)9899, (seq_t)30047, (seq_t)39946, (seq_t)69993,
            (seq_t)459904, (seq_t)529897, (seq_t)989801, (seq_t)3499300
        };

        /* fill the new entries */
        for (size_t i = cache_len; i < new_len; ++i) {
            if (i < 16)
                cache[i] = init[(size_t)i];
            else
                cache[i] = (seq_t)9998 * cache[i - 8] - cache[i - 16];
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
