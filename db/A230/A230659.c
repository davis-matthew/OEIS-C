/*
 * A230659  Number of n X 5 0..2 black square subarrays x(i,j) with each
 *          element diagonally or antidiagonally next to at least one element
 *          with value 2-x(i,j).
 * https://oeis.org/A230659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A230659
   Recurrence (valid for n > 10):
       a(n) = 59*a(n-2) - 230*a(n-4) - 2*a(n-6) + 32*a(n-8)
   Initial values:
       a(1)=0, a(2)=15, a(3)=135, a(4)=531, a(5)=8613,
       a(6)=28161, a(7)=477279, a(8)=1539207, a(9)=26178201,
       a(10)=84335553
   The function memoises computed terms so that any call order is
   supported. */

#include <stdlib.h>

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;      /* allocated capacity (index + 1) */
    static size_t stored = 0;     /* largest index computed so far */

    if (n < 1)                     /* undefined offset; return 0 */
        return 0;

    /* allocate more space if required */
    if (cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failed – cannot compute */
            return 0;
        }
        cache = new_cache;
        cap   = new_cap;
    }

    /* compute missing terms up to n */
    if (stored < (size_t)n) {
        /* initial values for indices 1 … 10 */
        static const seq_t init[10] = {
            0ULL, 15ULL, 135ULL, 531ULL, 8613ULL,
            28161ULL, 477279ULL, 1539207ULL, 26178201ULL, 84335553ULL
        };

        for (size_t i = stored + 1; i <= (size_t)n; ++i) {
            if (i <= 10) {
                cache[i] = init[i - 1];
            } else {
                seq_t val = 59 * cache[i - 2];
                val -= 230 * cache[i - 4];
                val -= 2   * cache[i - 6];
                val += 32  * cache[i - 8];
                cache[i] = val;
            }
        }
        stored = n;
    }

    return cache[n];
}
