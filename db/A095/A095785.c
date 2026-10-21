/*
 * A095785  Values of n for which A095777(n) is 16 (those terms which are
 *          expressible in decimal digits for bases 2 through 17, but not for
 *          base 18).
 * https://oeis.org/A095785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 18, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   valid_number
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A095785: values of n for which A095777(n) is 16.
   A085785(n) is the nth integer n (n ≥ 1) such that
   in every base b with 2 ≤ b ≤ 17 the base‑b representation of n
   uses only digits 0–9, and in base 18 it contains at least one digit
   ≥10 (i.e. a non‑decimal digit). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/valid_number.h"

/* static cache for memoisation of computed terms */
static seq_t *cache   = NULL;
static size_t cache_size  = 0;
static size_t cache_cap   = 0;

/* last number examined while incrementally building the sequence */
static seq_t last_checked = 0;

/* append a new term to the cache, growing the buffer as necessary */
static void append_term(seq_t val)
{
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();      /* out of memory */
        cache   = tmp;
        cache_cap = new_cap;
    }
    cache[cache_size++] = val;
}

/* Return the nth term of the sequence (1‑indexed). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0 */

    /* Incrementally generate terms until we have at least n of them */
    while (cache_size < n) {
        ++last_checked;
        if (valid_number(last_checked))
            append_term(last_checked);
    }
    return cache[n - 1];
}
