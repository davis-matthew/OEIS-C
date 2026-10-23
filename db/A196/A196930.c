/*
 * A196930  Triangle read by rows in which row n lists in nondecreasing order
 *          the smallest part of every partition of n that do not contain 1 as
 *          a part, with a(1) = 1.
 * https://oeis.org/A196930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n‑th element of the flattened triangle that, for
 *            each m ≥ 1, contains the smallest part of every partition of m
 *            that does not use the part 1, with the entries for a row sorted
 *            nondecreasing. The function builds the triangle row by row until
 *            it reaches the required index
 * Range:     u8 n <= 9138, u16 n <= 9138, u32 n <= 9138, u64 n <= 9138,
 *            u128 n <= 9138, big any n
 * Helpers:   cmp_seq_asc, gen_parts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "../utils/cmp_seq_asc.h"
#include "../utils/gen_parts.h"

/* Cache of the flattened triangle */
static seq_t *cache = NULL;          /* flattened values   */
static size_t cache_used = 0;        /* number of values   */
static size_t cache_cap = 0;         /* current capacity   */
static idx_t  current_row = 0;       /* last processed row */

/* Maximum number of parts that can occur in a partition of any n
   that we need. A safe bound for n≤10000 is 20000. */
#define MAX_PARTS 20000

/* Append one value to the cache, expanding it if necessary */
static void push_value(seq_t v)
{
    if (cache_used == cache_cap) {
        cache_cap = cache_cap ? cache_cap * 2 : 16;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_used++] = v;
}

/* Build rows until the cache contains at least n values */
static void build_until(idx_t n)
{
    while (cache_used < (size_t)n) {
        /* Row 1 is an exception: the only partition allowed is [1] */
        if (current_row == 0) {
            push_value((seq_t)1);
            current_row = 1;
            continue;
        }

        /* Generate next row */
        ++current_row;
        seq_t parts[MAX_PARTS];
        seq_t smallest[MAX_PARTS];
        size_t small_len = 0;

        /* For current_row ≥ 2, all parts must be ≥ 2. */
        gen_parts(current_row, current_row, parts, 0, smallest, &small_len);

        /* Sort the smallest parts to obtain the row in nondecreasing order */
        qsort(smallest, small_len, sizeof(seq_t), cmp_seq_asc);

        /* Append the sorted row to the cache */
        for (size_t i = 0; i < small_len; ++i)
            push_value(smallest[i]);
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but return 0 */

    build_until(n);
    return cache[n - 1];
}
