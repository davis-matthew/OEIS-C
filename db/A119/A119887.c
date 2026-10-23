/*
 * A119887  Numbers whose square starts with 6 identical digits.
 * https://oeis.org/A119887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A119887: numbers whose square starts with six identical digits.
   The algorithm generates the terms sequentially, caching them, and resumes
   from where it left off on subsequent calls.  It uses integer arithmetic only. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/floor_sqrt.h"

/* Cache of terms that have already been generated */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* State of the current block being processed */
static idx_t cur_m     = 6;   /* number of digits of the square (>=6)          */
static int  cur_d     = 1;    /* leading digit of the first six identical digits (1..9) */
static seq_t cur_x_min;      /* first integer whose square lies in the current block   */
static seq_t cur_x_max;      /* last integer whose square lies in the current block    */
static seq_t cur_x_next;     /* next integer to push from the current block           */
static bool  initialized = false;

/* Append a term to the cache, growing the storage if necessary */
static void push_term(seq_t v)
{
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();      /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_size++] = v;
}

/* Compute the bounds of the current block (cur_m, cur_d) */
static void compute_current_block(void)
{
    seq_t pow10_m6 = pow10(cur_m - 6);                 /* 10^(cur_m-6) */
    seq_t block    = pow10_m6 * 111111;               /* 111111 * 10^(cur_m-6) */
    seq_t A = (seq_t)cur_d * block;                   /* lower bound of the block */
    seq_t B = A + pow10_m6 - 1;                      /* upper bound of the block */

    seq_t x_min = floor_sqrt(A);
    if (x_min * x_min < A) ++x_min;                  /* ceil sqrt(A) */
    seq_t x_max = floor_sqrt(B);                     /* floor sqrt(B) */

    cur_x_min  = x_min;
    cur_x_max  = x_max;
    cur_x_next = cur_x_min;
}

/* Generate terms until the cache holds at least target terms */
static void generate_up_to(idx_t target)
{
    if (!initialized) {
        cur_m = 6;
        cur_d = 1;
        compute_current_block();
        initialized = true;
    }

    while (cache_size < (size_t)target) {
        if (cur_x_next <= cur_x_max) {
            push_term(cur_x_next);
            ++cur_x_next;
        } else {
            /* Move to the next block */
            ++cur_d;
            if (cur_d > 9) {
                cur_d = 1;
                ++cur_m;
            }
            compute_current_block();
        }
    }
}

/* Public function required by the exercise */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* a(0) is out of the OEIS offset */
    if (cache_size < (size_t)n) generate_up_to(n);
    return cache[n - 1];
}
