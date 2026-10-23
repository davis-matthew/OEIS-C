/*
 * A028755  Nonsquares mod 42.
 * https://oeis.org/A028755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th positive integer that is not a quadratic residue modulo 42 */

/* quadratic residues modulo 42:
   0, 1, 4, 7, 9, 15, 16, 18, 21, 22, 25, 28, 30, 36, 37, 39 */
static const unsigned char is_square_mod42[42] = {
    1, /* 0 */
    1, /* 1 */
    0, /* 2 */
    0, /* 3 */
    1, /* 4 */
    0, /* 5 */
    0, /* 6 */
    1, /* 7 */
    0, /* 8 */
    1, /* 9 */
    0, /*10*/
    0, /*11*/
    0, /*12*/
    0, /*13*/
    0, /*14*/
    1, /*15*/
    1, /*16*/
    0, /*17*/
    1, /*18*/
    0, /*19*/
    0, /*20*/
    1, /*21*/
    1, /*22*/
    0, /*23*/
    0, /*24*/
    1, /*25*/
    0, /*26*/
    0, /*27*/
    1, /*28*/
    0, /*29*/
    1, /*30*/
    0, /*31*/
    0, /*32*/
    0, /*33*/
    0, /*34*/
    0, /*35*/
    1, /*36*/
    1, /*37*/
    0, /*38*/
    1, /*39*/
    0, /*40*/
    0  /*41*/
};

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for already computed terms */
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;
    static idx_t  cache_cap   = 0;
    static idx_t  next_check   = 1;   /* next integer to examine */

    /* ensure cache has at least n entries */
    while (cache_size < n) {
        /* grow cache capacity if needed */
        if (cache_size >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);          /* allocation failed */
            cache = tmp;
            cache_cap = (idx_t)new_cap;
        }

        /* find next non‑residue modulo 42 */
        while (true) {
            idx_t rem = next_check % 42;
            if (!is_square_mod42[rem]) {
                cache[cache_size++] = (seq_t)next_check;
                ++next_check;        /* advance to the next integer */
                break;
            }
            ++next_check;
        }
    }

    return cache[n - 1];
}
