/*
 * A052052  Cubes whose digits occur with an equal minimal frequency of 2.
 * https://oeis.org/A052052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   cube_has_equal_freq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A052052
 * Search for the n-th perfect cube whose decimal digits
 * all occur with the same frequency, that frequency being
 * at least 2.  The sequence is stored 1‑indexed (a(1) is the
 * first term).  The function memoises results but does not
 * assume calls are in increasing order.  Works for all
 * supported seq_t types (8–128 bit integers and the
 * arbitrary‑precision class). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/cube_has_equal_freq.h"

/* Ensure *arr has room for at least `need` elements.  The
 * array is reallocated with doubling growth.  `arr` must
 * be a pointer to a seq_t*; `cap` is the current capacity. */
static void ensure_cap(size_t *cap, size_t need, seq_t **arr)
{
    if (*cap == 0) *cap = 16;
    while (*cap < need) *cap <<= 1;
    *arr = (seq_t *)realloc(*arr, *cap * sizeof(seq_t));
}

/* Return the n-th term of the sequence (n ≥ 1).  For n < 1
 * the behaviour is undefined, but the function returns 0
 * for safety. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            /* safety for n = 0 */

    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_size = 0;
    static idx_t last_root   = 0;

    while (cache_size < (size_t)n) {
        /* Next cube to test. */
        last_root++;
        seq_t r = (seq_t)last_root;
        seq_t cube = r * r * r;

        if (cube_has_equal_freq(cube)) {
            if (cache_size >= cache_cap)
                ensure_cap(&cache_cap, cache_size + 1, &cache);
            cache[cache_size++] = cube;
        }
    }

    return cache[n - 1];
}
