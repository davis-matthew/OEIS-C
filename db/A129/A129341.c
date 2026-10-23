/*
 * A129341  Numbers k such that k-th and (k+1)-th triangular numbers have no
 *          common digit.
 * https://oeis.org/A129341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   no_common_digits_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A129341: numbers k such that the k‑th and (k+1)-th triangular
   numbers have no common decimal digit.  The function generates the
   values by brute‑force search. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/no_common_digits_uint128.h"

/* Cache for the computed sequence values. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Next k to test. */
static idx_t next_k = 0;

/* Ensure the cache holds at least new_cap elements. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    size_t new_size = (cache_cap == 0) ? 16 : cache_cap << 1;
    while (new_size < new_cap)
        new_size <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        exit(1);                     /* allocation failure */
    cache = tmp;
    cache_cap = new_size;
}

/* Generate terms until the cache contains at least n terms. */
static void ensure_terms(idx_t n)
{
    while (cache_len < (size_t)n) {
        idx_t k = next_k;
        unsigned __int128 Tk  = (unsigned __int128)k * (k + 1) / 2;
        unsigned __int128 Tk1 = (unsigned __int128)(k + 1) * (k + 2) / 2;
        if (no_common_digits_uint128(Tk, Tk1)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)k;
        }
        next_k = k + 1;
    }
}

/* Required external function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for n=0; safe default */
    ensure_terms(n);
    return cache[n - 1];
}
