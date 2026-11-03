/*
 * A208842  Number of 5 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A208842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 209 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS Sequence A208842
   Recurrence: a(n) = 2*a(n-1) + 12*a(n-2) - 11*a(n-3)
   Initial values: a(1)=14, a(2)=196, a(3)=406
   (Any n ≥ 1 is computed; calls may be in any order.)
*/

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

seq_t a(idx_t n)
{
    /*  mutable cache for computed terms  */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;  /* number of elements allocated  */
    static bool initialized = false;

    if (n < 1) return (seq_t)0;  /* undefined for n=0, safety */

    /*  Ensure space for indices 0 .. n  (index 0 unused)  */
    if ((size_t)n + 1 > cache_size) {
        grow_seq_array((seq_t **)&cache, &cache_size, (size_t)n + 1);
    }

    /*  Initialize base values once  */
    if (!initialized) {
        cache[1] = (seq_t)14;
        cache[2] = (seq_t)196;
        cache[3] = (seq_t)406;
        initialized = true;
    }

    /*  If result already computed, return it  */
    if ((size_t)n < cache_size && cache[n] != (seq_t)0) {
        return cache[n];
    }

    /*  Compute terms up to n using the recurrence  */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t term = 2 * cache[i - 1];
        term += 12 * cache[i - 2];
        term -= 11 * cache[i - 3];
        cache[i] = term;
    }

    return cache[n];
}
