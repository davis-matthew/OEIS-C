/*
 * A043380  Numbers having four 2's in base 6.
 * https://oeis.org/A043380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer whose base‑6 expansion contains
 *            exactly four digits equal to 2. The values are generated in
 *            increasing order by scanning consecutive integers and counting
 *            the digit 2 in base‑6. The first term is a(1)=518
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   has_four_twos
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_four_twos.h"

/* --- static cache --------------------------------------------------- */
static seq_t *CACHE      = NULL;          /* sorted list of found terms   */
static size_t CACHE_CAP  = 0;             /* allocated capacity           */
static size_t CACHE_SIZE = 0;             /* number of stored terms       */
static idx_t  TESTED_UPTO = 0;            /* largest integer tested       */

/* --- utility: add a value to the cache ------------------------------ */
static void cache_push(seq_t value)
{
    if (CACHE_CAP <= CACHE_SIZE) {
        size_t new_cap = CACHE_CAP ? CACHE_CAP * 2 : 16;
        while (new_cap <= CACHE_SIZE) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(CACHE, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();   /* ensure memory availability */
        CACHE = tmp;
        CACHE_CAP = new_cap;
    }
    CACHE[CACHE_SIZE++] = value;
}

/* --- extend the cache until it contains at least n elements -------- */
static void ensure_cache(idx_t n)
{
    if (CACHE_SIZE >= (size_t)n) return;

    /* keep scanning numbers until enough terms are collected */
    while (CACHE_SIZE < (size_t)n) {
        uint64_t val = (uint64_t)TESTED_UPTO + 1;
        if (has_four_twos(val)) {
            cache_push((seq_t)val);    /* cast is safe for unsigned */
        }
        TESTED_UPTO = (idx_t)val;
    }
}

/* --- public entry point --------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_cache(n);
    return CACHE[n - 1];
}
