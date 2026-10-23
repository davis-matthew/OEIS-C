/*
 * A196990  Numbers that are not the sum of two powers of 3.
 * https://oeis.org/A196990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers that are NOT the sum of two powers of 3.
   The function repeatedly tests each integer, collecting those
   for which no pair (3^i + 3^j) equals the candidate. */

/* Include standard headers. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- static helper data ---------- */

/* Dynamically growing array of powers of 3. */
static seq_t *powers = NULL;      /* pointer to powers array */
static size_t powers_cap = 0;     /* capacity of powers array     */
static size_t powers_len = 0;     /* number of elements in powers */
static seq_t next_power = (seq_t)1;  /* next power of 3 to add */

/* Cache of previously found terms of the sequence. */
static seq_t *terms = NULL;      /* pointer to terms array  */
static size_t terms_cap = 0;     /* capacity of terms array   */
static size_t terms_len = 0;     /* number of terms stored    */
static seq_t cur_val = (seq_t)1; /* next integer to test     */

/* ---------- helper functions ---------- */

/* Ensure that the array `powers` contains all powers of 3 ≤ `val`.  */
static void
ensure_powers_leq(seq_t val)
{
    while (next_power <= val) {
        /* Grow the powers array if necessary. */
        if (powers_len == powers_cap) {
            size_t new_cap = powers_cap ? powers_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(powers, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            powers = tmp;
#ifdef __cplusplus
            /* Default‑construct any newly allocated elements. */
            for (size_t i = powers_cap; i < new_cap; ++i)
                new((void *)&powers[i]) seq_t();
#endif
            powers_cap = new_cap;
        }
        powers[powers_len++] = next_power;
        next_power = next_power * ((seq_t)3);
    }
}

/* Test whether `val` can be written as the sum of two (possibly equal)
   powers of 3.  Assumes `powers` already contains all powers ≤ `val`. */
static bool
is_sum_two_powers(seq_t val)
{
    for (size_t i = 0; i < powers_len; ++i) {
        seq_t p = powers[i];
        if (p > val) break;
        seq_t rem = val - p;

        /* binary search for `rem` in the powers array. */
        size_t low = 0, high = powers_len;
        while (low < high) {
            size_t mid = low + (high - low) / 2;
            if (powers[mid] < rem)
                low = mid + 1;
            else
                high = mid;
        }
        if (low < powers_len && powers[low] == rem)
            return true;
    }
    return false;
}

/* ---------- main sequence function ---------- */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n=0, harmless */

    /* Generate additional terms until we have at least `n` of them. */
    while (terms_len < (size_t)n) {
        ensure_powers_leq(cur_val);

        if (!is_sum_two_powers(cur_val)) {
            /* Grow the terms array if necessary. */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                terms = tmp;
#ifdef __cplusplus
                /* Default‑construct any newly allocated elements. */
                for (size_t i = terms_cap; i < new_cap; ++i)
                    new((void *)&terms[i]) seq_t();
#endif
                terms_cap = new_cap;
            }
            terms[terms_len++] = cur_val;
        }
        ++cur_val;
    }

    return terms[n - 1];   /* 1‑based indexing */
}
