/*
 * A044665  Numbers n such that string 3,6 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A044665: a(n) = nth integer whose base‑9 representation
   contains the substring 36, while (n+1) does not. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static bool contains36(seq_t num)
{
    bool first = true;
    uint64_t prev = 0;          /* previous digit in base‑9, stored small */
    while (num > 0) {
        uint64_t d = (uint64_t)(num % 9);   /* current base‑9 digit   */
        num = num / 9;
        if (!first && prev == 6 && d == 3)   /* found "36"           */
            return true;
        prev = d;
        first = false;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static seq_t next_candidate = 1;

    if (n == 0)
        return 0;   /* undefined for offset 1, harmless for memoisation */

    /* Allocate / grow cache as needed */
    if (cache_size < (size_t)n) {
        size_t need = (size_t)n;
        if (cache_cap < need) {
            size_t new_cap = cache_cap ? cache_cap : 16;
            while (new_cap < need)
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)   /* allocation failure */
                return 0;
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Generate terms until cache_size reaches n */
        while (cache_size < need) {
            if (contains36(next_candidate) &&
                !contains36(next_candidate + 1)) {
                cache[cache_size++] = next_candidate;
            }
            next_candidate++;
        }
    }
    return cache[n - 1];
}
