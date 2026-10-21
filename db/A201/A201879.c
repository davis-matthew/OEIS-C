/*
 * A201879  Numbers n such that sigma_2(n) - n^2 is a square.
 * https://oeis.org/A201879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 45, u32 n <= 2604, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A201879: numbers n such that the sum of squares of proper divisors of n is a square.
   Computation is performed by testing integers in ascending order and recording the ones
   that satisfy the condition. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "../utils/sigma_k.h"
#include "../utils/floor_sqrt.h"

/* Cache of the sequence values found so far. */
static seq_t *seq_vals = NULL;   /* array of sequence terms */
static size_t seq_len = 0;       /* number of terms stored */
static size_t seq_cap = 0;       /* current capacity of seq_vals */
static idx_t  last_tested = 1;   /* next integer to test */

/* Append a term to the cache, growing the array if necessary. */
static void seq_append(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq_t *new_arr = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
        /* realloc may return NULL only if out of memory; the build environment
           is expected to have sufficient memory for this task. */
        seq_vals = new_arr;
        seq_cap  = new_cap;
    }
    seq_vals[seq_len++] = val;
}

/* Check whether integer n satisfies: sum of squares of proper divisors of n is a square. */
static bool is_good(idx_t n)
{
    /* sigma_k returns the sum of the k-th powers of all divisors. */
    seq_t sigma_sq = sigma_k(n, (uint8_t)2);   /* sum of squares of all divisors */
    seq_t nn        = (seq_t)n;
    seq_t delta     = sigma_sq - nn * nn;      /* sum of squares of proper divisors */

    seq_t root      = floor_sqrt(delta);
    return (root * root == delta);
}

/* External function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; a(0) is irrelevant */

    /* Generate sequence terms until we have at least n of them. */
    while (seq_len < (size_t)n) {
        if (is_good(last_tested))
            seq_append((seq_t)last_tested);
        ++last_tested;
    }

    return seq_vals[(size_t)n - 1];
}
