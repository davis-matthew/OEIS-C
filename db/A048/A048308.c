/*
 * A048308  Numbers whose base-11 expansions, read from left to right, have
 *          run lengths that strictly increase.
 * https://oeis.org/A048308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A048308: Numbers whose base‑11 expansions, read from left to right,
   have run lengths that strictly increase.
   Implementation: brute‑force enumeration of all base‑11 digit strings that
   satisfy the run‑length condition, converted to decimal values, and cached
   in ascending order.  The function a(n) returns the n‑th smallest such
   number (a(0)=0).  The algorithm works for any n; it generates lengths
   incrementally until the required term is produced. */

#include <stdlib.h>  /* malloc, realloc */

/* static cache of generated terms */
static seq_t *cache      = NULL;
static size_t cache_size = 0;   /* number of terms stored */
static size_t cache_cap  = 0;   /* allocated capacity */
static size_t max_len_generated = 0;   /* largest length that has been generated */

/* ensure the cache has space for at least 'need' terms */
static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap < need) new_cap <<= 1;
    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = p;
    cache_cap = new_cap;
}

/* recursive generator for all base‑11 strings of length L
   satisfying strictly increasing run lengths */
static void gen_len(int L, int pos, int prev_digit,
                    int prev_run_len, int curr_run_len, seq_t value)
{
    if (pos == L) {
        /* finished a string – accept if last run is longer than previous */
        if (curr_run_len > prev_run_len) {
            if (cache_size == cache_cap) grow_cache(cache_size + 1);
            cache[cache_size++] = value;
        }
        return;
    }

    if (pos == 0) {
        /* first digit: cannot be 0, start a new run of length 1 */
        for (int d = 1; d < 11; ++d) {
            seq_t new_val = (seq_t)d;
            gen_len(L, 1, d, 0, 1, new_val);
        }
    } else {
        for (int d = 0; d < 11; ++d) {
            seq_t new_val = (value * (seq_t)11) + (seq_t)d;
            if (d == prev_digit) {
                /* continue current run */
                gen_len(L, pos + 1, prev_digit, prev_run_len,
                        curr_run_len + 1, new_val);
            } else {
                /* start a new run – only allowed if previous run is longer
                   than the one before it (prev_run_len) */
                if (curr_run_len > prev_run_len) {
                    gen_len(L, pos + 1, d, curr_run_len, 1, new_val);
                }
            }
        }
    }
}

/* external function required by the interface */
seq_t a(idx_t n)
{
    /* initialise cache once */
    if (!cache) {
        grow_cache(1);
        cache[0] = 0;          /* a(0) = 0 */
        cache_size = 1;
        max_len_generated = 0;
    }

    /* generate more terms until we have at least n+1 elements */
    while (cache_size <= n) {
        ++max_len_generated;
        gen_len((int)max_len_generated, 0, 0, 0, 0, (seq_t)0);
    }

    return cache[(size_t)n];
}
