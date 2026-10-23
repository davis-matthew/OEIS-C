/*
 * A177376  Fibonacci numbers whose decimal expansion does not contain any
 *          digit 9.
 * https://oeis.org/A177376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 29, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursively generate Fibonacci numbers and keep those whose
   decimal representation contains no digit 9. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_frequencies.h"

/* Return true iff the decimal digits of value contain no '9'. */
static bool has_no_nine(seq_t value)
{
    unsigned int freq[10];
    digit_frequencies(value, freq);
    return freq[9] == 0;
}

/* a(n) = the n‑th Fibonacci number whose decimal expansion has no
   digit 9, for n ≥ 1.  The sequence is finite (last term is
   F(188)). */
seq_t a(idx_t n)
{
    /* static cache of found Fibonacci numbers without a 9 */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    /* static state for Fibonacci generation */
    static seq_t f_prev = 0;   /* F(k-1) */
    static seq_t f_curr = 1;   /* F(k)   */
    static idx_t   k      = 1; /* current index of f_curr */

    /* initialise cache and state on first call */
    if (!cache) {
        cap   = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        f_prev = 0;
        f_curr = 1;
        k      = 1;
    }

    /* Generate Fibonacci numbers until we have at least n valid ones
       or we reach a practical hard limit (F(2000)). */
    while (len < (size_t)n) {
        if (has_no_nine(f_curr)) {
            if (len >= cap) {
                cap <<= 1;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = f_curr;
        }

        seq_t f_next = f_prev + f_curr;
        /* Detect unsigned overflow: if the addition wrapped around,
           further computation would be invalid for this seq_t type. */
        if (f_next < f_curr) break;
        f_prev = f_curr;
        f_curr = f_next;
        ++k;

        /* Safety cut‑off – after F(2000) all known values are exhausted. */
        if (k > 2000ULL) break;
    }

    /* Return the requested term if available. */
    if (n <= len)
        return cache[n - 1];

    /* Requested index lies beyond the defined sequence. */
    return (seq_t)0;
}
