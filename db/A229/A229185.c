/*
 * A229185  Numbers incremented by their digit product produce a cube.
 * https://oeis.org/A229185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = the n‑th nonnegative integer x such that x +
 *            digit_product(x) is a perfect cube (x + Π digits(x) = m³). The
 *            sequence starts with a(1)=0, a(2)=4, a(3)=85, ... as given in
 *            OEIS A229185
 * Range:     u8 n <= 2, u16 n <= 29, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   digit_product, floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_product.h"
#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cached terms, 0‑based */
    static size_t cache_len = 0;     /* number of cached terms */
    static size_t cache_cap = 0;     /* allocated capacity of cache */
    static seq_t next_to_check = 0;  /* next candidate integer */

    if (n == 0) return 0; /* undefined, but return sensible value */

    while (cache_len < n) {
        seq_t num  = next_to_check;                      /* candidate */
        seq_t prod = digit_product(num);
        seq_t sum  = num + prod;                         /* n + product(n) */
        seq_t c    = floor_cube_root(sum);               /* cube root candidate */
        seq_t cube = c * c * c;                           /* c³, safe in seq_t */

        if (cube == sum) {                                /* perfect cube? */
            if (cache_len == cache_cap) {                 /* grow cache */
                size_t new_cap = (cache_cap == 0) ? 16 : (cache_cap << 1);
                seq_t *newp = (seq_t *)realloc(cache,
                                                new_cap * sizeof(seq_t));
                cache = newp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = num;                     /* store term */
        }
        ++next_to_check;
    }

    return cache[n - 1];   /* 1‑based index */
}
