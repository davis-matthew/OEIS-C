/*
 * A069861  Numbers n that divide the concatenation of next n numbers.
 * https://oeis.org/A069861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   pow_mod_10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A069861: Numbers n that divide the concatenation of the next n numbers.
   The function a(n) returns the n‑th term of the sequence (offset 1).
   The algorithm tests successive integers, computes the remainder of the
   concatenation modulo that integer, and records it when the remainder is 0. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/pow_mod_10.h"

/* ---------- Static helpers ---------------------------------------------- */
/* Return the number of decimal digits of an unsigned integer. */
static int digits_uint64(uint64_t v)
{
    int d = 0;
    do {
        d++;
        v /= 10;
    } while (v);
    return d;
}

/* ---------- Cache for the sequence ------------------------------------- */
static seq_t *cache = NULL;   /* array of already found terms   */
static size_t cache_len = 0;  /* number of terms stored         */
static size_t cache_cap = 0;  /* current allocation capacity    */

/* Append a new term to the cache, extending the allocation if necessary. */
static void append_term(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_arr)
            abort();  /* out of memory */
        cache = new_arr;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* ---------- Main function ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; caller must use n>=1      */
        abort();

    /* If already computed, return immediately. */
    if (cache_len >= n)
        return cache[n - 1];

    /* Start searching from the next candidate after the last cached term. */
    idx_t candidate = (cache_len == 0) ? 1 : (idx_t)(cache[cache_len - 1] + 1);

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        seq_t m = (seq_t)candidate;          /* current candidate value   */
        seq_t rem = 0;                       /* remainder modulo m       */

        idx_t limit = candidate + candidate; /* last number to concatenate */

        for (idx_t k = candidate + 1; k <= limit; ++k) {
            seq_t km = (seq_t)k;
            int d = digits_uint64((uint64_t)k);   /* digits of k               */
            seq_t pow10mod = pow_mod_10(d, m);    /* 10^d mod m                 */
            rem = ( (rem * pow10mod) % m + (km % m) ) % m;
        }

        if (rem == 0)                 /* property satisfied             */
            append_term(m);

        ++candidate;
    }

    return cache[n - 1];
}
