/*
 * A043802  Numbers n such that number of runs in the base 3 representation of
 *          n is congruent to 4 mod 8.
 * https://oeis.org/A043802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n‑th integer whose number of runs in the base‑3
 * representation of n is congruent to 4 (mod 8).  Computed by brute‑force
 * search with memoization. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* test whether the base‑3 representation of num has a number of runs
   congruent to 4 modulo 8.  Runs are maximal blocks of identical
   consecutive digits in base 3. */
static bool base3_runs_congruent4_mod8(seq_t num)
{
    int runs = 0;
    int prev_digit = -1;           /* not a valid digit */
    seq_t tmp = num;

    /* Special case for zero: base‑3 representation is "0" → 1 run. */
    if (tmp == (seq_t)0) {
        return (1 % 8) == 4;
    }

    /* Divide by 3 repeatedly, counting digit changes (runs). */
    while (tmp > (seq_t)0) {
        int digit = (int)(tmp % (seq_t)3);
        if (digit != prev_digit) {
            ++runs;
            prev_digit = digit;
        }
        tmp /= (seq_t)3;
    }
    return (runs % 8) == 4;
}

/* Cached sequence data.  All static variables are used only inside this
   translation unit to accumulate computed terms up to the largest
   requested index. */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_test = 0;   /* next integer to be examined */

/* Return the n-th term (n >= 1) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) {                /* offset is 1; undefined for 0 */
        return (seq_t)0;
    }

    /* Extend the cache until it contains the requested term. */
    while (cache_len < (size_t)n) {
        if (base3_runs_congruent4_mod8((seq_t)next_test)) {
            /* Ensure enough capacity. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) {          /* allocation failed */
                    return (seq_t)0; /* fallback */
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_test;
        }
        /* Move to the next integer to test. */
        ++next_test;
    }

    /* Sequence is 1‑indexed; return the stored term. */
    return cache[(size_t)n - 1];
}
