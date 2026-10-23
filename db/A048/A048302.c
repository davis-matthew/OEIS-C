/*
 * A048302  Numbers whose base-5 expansions, read from left to right, have run
 *          lengths that strictly increase.
 * https://oeis.org/A048302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth smallest non‑negative integer whose base‑5 expansion,
 *            when read left‑to‑right, has run lengths that are strictly
 *            increasing. The function enumerates all such numbers up to a
 *            bound on the number of digits, sorts them, and indexes the
 *            resulting ascending list
 * Range:     u8 n <= 0, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   cmp_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*======================================================================
  Includes and type/size definitions
  =====================================================================*/
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/cmp_seq_t.h"

/*======================================================================
  Compile‑time determination of the maximum number of base‑5 digits
  that can be safely used for this build type.
  For 8‑bit & 16‑bit types: 6 digits (5^6 < 2^16).
  For 32‑bit types:          13 digits (5^13 < 2^32-1).
  For 64‑bit and larger:     20 digits (5^20 < 2^64-1).
  =====================================================================*/
static const size_t MAX_DIGITS =
   (sizeof(seq_t) <= 2) ? 6 :
   (sizeof(seq_t) <= 4) ? 13 :
   20;

/*======================================================================
  Recursive generator that fills an output buffer with all numbers
  whose base‑5 run lengths increase strictly.
  =====================================================================*/
static void gen_rec(int cur_len,
                    int prev_len,
                    seq_t value,
                    int prev_digit,
                    seq_t *out,
                    size_t *len,
                    size_t max_digits,
                    const seq_t *pow5)
{
    /* Try all possible next run lengths L greater than the last one. */
    for (int L = prev_len + 1; L <= (int)(max_digits - cur_len); ++L) {
        const seq_t powL = pow5[L];
        const seq_t term = (powL - (seq_t)1) / (seq_t)4;  /* 1 + 5 + ... + 5^(L-1) */

        /* Try all possible digits for this run. */
        for (int d = 0; d <= 4; ++d) {
            if (prev_digit >= 0 && d == prev_digit)   continue;   /* no two equal runs */
            if (prev_digit < 0 && d == 0)            continue;   /* no leading zero */
            const seq_t new_val = value * powL + ((seq_t)d) * term;
            out[(*len)++] = new_val;
            gen_rec(cur_len + L,
                    L,
                    new_val,
                    d,
                    out,
                    len,
                    max_digits,
                    pow5);
        }
    }
}

/*======================================================================
  Builds a sorted cache of all valid numbers up to a given maximum
  number of base‑5 digits.  The array contains the value 0 as the
  first element.
  =====================================================================*/
static seq_t *build_cache(size_t max_digits, size_t *len_out)
{
    /* Pre‑allocate a generous buffer.  With MAX_DIGITS=20 the count stays
       well below 200 000 for built‑in types.  For smaller types the
       generated value set is far smaller. */
    size_t buf_cap = 200000;
    seq_t *buffer = (seq_t*)malloc(buf_cap * sizeof(seq_t));
    if (!buffer) return NULL;

    /* Pre‑compute powers of 5 up to max_digits. */
    seq_t *pow5 = (seq_t*)malloc((max_digits + 1) * sizeof(seq_t));
    if (!pow5) { free(buffer); return NULL; }
    pow5[0] = (seq_t)1;
    for (size_t i = 1; i <= max_digits; ++i) {
        pow5[i] = pow5[i-1] * (seq_t)5;
    }

    /* Generate all numbers with at most max_digits base‑5 digits. */
    size_t count = 0;
    gen_rec(0, 0, (seq_t)0, -1, buffer, &count, max_digits, pow5);

    /* Append the number 0 (no leading digit). */
    if (count >= buf_cap) {
        /* This should never happen with the chosen buffer size. */
        buf_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(buffer, buf_cap * sizeof(seq_t));
        if (!tmp) { free(buffer); free(pow5); return NULL; }
        buffer = tmp;
    }
    buffer[count++] = (seq_t)0;

    /* Sort the list. */
    qsort(buffer, count, sizeof(seq_t), cmp_seq_t);

    *len_out = count;
    free(pow5);
    return buffer;
}

/*======================================================================
  Static cache and helper to (re)build it as needed.
  =====================================================================*/
static seq_t *cache       = NULL;
static size_t cache_len   = 0;   /* number of elements in cache   */
static size_t cache_digits = 0;  /* maximum digits used in cache */

/* Rebuild the cache with an increased digit bound until it contains
   at least n+1 elements.  The first build uses MAX_DIGITS. */
static void ensure_cache(idx_t n)
{
    if (cache && n < cache_len) return;

    size_t new_digits = (cache_digits == 0) ? MAX_DIGITS : cache_digits + 5;
    while (1) {
        size_t new_len = 0;
        seq_t *new_cache = build_cache(new_digits, &new_len);
        if (!new_cache) return;          /* allocation failure → keep old cache */

        free(cache);
        cache = new_cache;
        cache_len   = new_len;
        cache_digits = new_digits;

        if (n < cache_len) break;
        new_digits += 5;
    }
}

/*======================================================================
  Public routine required by the OEIS harness
  =====================================================================*/
seq_t a(idx_t n)
{
    ensure_cache(n);
    if (n >= cache_len) return (seq_t)0;  /* out of bounds (should not happen) */
    return cache[n];
}
