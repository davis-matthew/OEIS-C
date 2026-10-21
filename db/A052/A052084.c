/*
 * A052084  Cubes with at least one of the decimal digits occurring separated.
 * https://oeis.org/A052084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 12, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   cube_has_separated_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth cube having at least one decimal digit that occurs
   separated (i.e., at least one digit appears twice in the
   decimal representation with at least one different digit between
   the two occurrences).  This is the OEIS sequence A052084. */
/* The implementation stores previously computed terms in a dynamic
   array so that multiple calls to a() in any order are efficient. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/cube_has_separated_digits.h"

static seq_t *a_cache = NULL;
static idx_t a_cache_size = 0;
static idx_t a_cache_cap  = 0;
static idx_t next_i = 1;

static void grow_cache(idx_t needed)
{
    if (a_cache_cap >= needed)
        return;
    idx_t new_cap = a_cache_cap ? a_cache_cap * 2 : 4;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!new_arr)
        exit(EXIT_FAILURE); /* allocation failure */
    a_cache = new_arr;
    a_cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* 1-indexed sequence */
    if (n == (idx_t)0)
        return (seq_t)0; /* undefined, return 0 */

    while (a_cache_size < n) {
        seq_t i = (seq_t)next_i;
        seq_t cube = i * i * i;
        if (cube_has_separated_digits(cube)) {
            grow_cache(a_cache_size + 1);
            a_cache[a_cache_size++] = cube;
        }
        ++next_i;
    }
    return a_cache[n - 1];
}
