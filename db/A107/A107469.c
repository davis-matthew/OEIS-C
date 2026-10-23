/*
 * A107469  1-limiting word of the morphism 1->112343211, 2->222444222,
 *          3->333444333, 4->444444444.
 * https://oeis.org/A107469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th symbol of the 1-limiting word of the morphism
 *            1→112343211, 2→222444222, 3→333444333, 4→444444444. The word is
 *            obtained by starting from the single symbol 1 and repeatedly
 *            replacing each symbol by its image. Since all images have length
 *            9, the n-th symbol can be found recursively b...
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base case: the word begins with symbol 1. */
    if (n == 0)
        return (seq_t)1;

    /* Precomputed expansions of each symbol (index 0 unused). */
    static const seq_t expand[5][9] = {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0 },                 /* dummy */
        { 1, 1, 2, 3, 4, 3, 2, 1, 1 },                 /* 1 → 112343211 */
        { 2, 2, 2, 4, 4, 4, 2, 2, 2 },                 /* 2 → 222444222 */
        { 3, 3, 3, 4, 4, 4, 3, 3, 3 },                 /* 3 → 333444333 */
        { 4, 4, 4, 4, 4, 4, 4, 4, 4 }                  /* 4 → 444444444 */
    };

    idx_t r      = n % 9;             /* position within the parent’s image */
    seq_t parent = a(n / 9);          /* symbol at the higher level */

    /* parent is in {1,2,3,4}; cast it to an integer type for indexing. */
    return expand[(unsigned long long)parent][r];
}
