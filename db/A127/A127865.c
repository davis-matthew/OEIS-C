/*
 * A127865  Number of square tiles in all tilings of a 2 X n board with 1 X 1
 *          and L-shaped tiles (where the L-shaped tiles cover 3 squares).
 * https://oeis.org/A127865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A127865 – Number of square tiles in all tilings of a 2×n board
 * with 1×1 and L‑shaped tiles.
 *
 * Recurrence (derived from the generating function):
 *   a(1)=2,  a(2)=8,  a(3)=30,  a(4)=108,  a(5)=354,  a(6)=1152
 *   for n≥7:
 *     a(n) = 2*a(n‑1) + 7*a(n‑2) − 4*a(n‑3) − 20*a(n‑4) − 16*a(n‑5) − 4*a(n‑6);
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* zero or negative indices */
    if (n == 0) return (seq_t)0;

    /* small values are returned directly */
    switch (n) {
        case 1: return (seq_t)2;
        case 2: return (seq_t)8;
        case 3: return (seq_t)30;
        case 4: return (seq_t)108;
        case 5: return (seq_t)354;
        case 6: return (seq_t)1152;
        default: break; /* fall through to recurrence for n>=7 */
    }

    /* static memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* current allocated size */
    static idx_t max_computed = 0; /* highest index computed so far */

    /* allocate or resize cache if necessary */
    if (cache == NULL) {
        cache_size = n + 1;
        cache = (seq_t *)calloc((size_t)cache_size, sizeof(seq_t));
        if (!cache) abort();           /* allocation failure */
        /* initialise base values */
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)30;
        cache[4] = (seq_t)108;
        cache[5] = (seq_t)354;
        cache[6] = (seq_t)1152;
        max_computed = 6;
    } else if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_size = new_size;
        /* base values remain valid */
    }

    /* compute missing terms up to n */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* use signed 128‑bit intermediate to avoid overflow */
            signed __int128 temp = 0;
            temp += (signed __int128)cache[i - 1] * 2;
            temp += (signed __int128)cache[i - 2] * 7;
            temp -= (signed __int128)cache[i - 3] * 4;
            temp -= (signed __int128)cache[i - 4] * 20;
            temp -= (signed __int128)cache[i - 5] * 16;
            temp -= (signed __int128)cache[i - 6] * 4;
            cache[i] = (seq_t)temp;
        }
        max_computed = n;
    }

    return cache[n];
}
