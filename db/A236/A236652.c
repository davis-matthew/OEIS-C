/*
 * A236652  Positive integers n such that n^2 divided by the digital root of n
 *          is a square.
 * https://oeis.org/A236652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 38, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 1000 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A236652 – Positive integers n such that (n² / digital_root(n)) is a perfect square.
   This implementation generates terms by brute‑force testing of consecutive integers
   and caches the results so that any random access a(n) is fast. */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, realloc, exit */
#include <stdbool.h>  /* for bool */

/* Helper from the utils: computes floor(sqrt(x)) for seq_t. */
#include <stdint.h>
#include <string.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* seq_mul multiplies two seq_t values. */

/* Static cache for the sequence terms. */
static seq_t *seq_cache = NULL;   /* array of found terms               */
static size_t  seq_cap  = 0;      /* current allocation capacity        */
static size_t  seq_len  = 0;      /* number of terms cached so far      */
static idx_t   next_int = 1;      /* next positive integer to test      */

/* Expand the cache as needed. */
static void ensure_cache(idx_t n)
{
    while (seq_len < n) {
        idx_t cand   = next_int;
        idx_t dr     = 1 + ((cand - 1) % 9);        /* digital root */

        seq_t prod   = seq_mul((seq_t)cand, (seq_t)cand);     /* cand²            */
        seq_t q      = prod / (seq_t)dr;                      /* division         */
        seq_t prod_r = seq_mul(q, (seq_t)dr);                 /* reconstruction   */

        if (prod_r == prod) {    /* exact division */
            seq_t root = floor_sqrt(q);
            if (seq_mul(root, root) == q) {   /* q is a square  */
                /* grow cache if full */
                if (seq_len == seq_cap) {
                    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
                    seq_t *tmp = mem_resize(seq_t, seq_cache, new_cap);
                    if (!tmp) { /* allocation failed – abort */
                        exit(EXIT_FAILURE);
                    }
                    seq_cache = tmp;
                    seq_cap   = new_cap;
                }
                seq_cache[seq_len++] = (seq_t)cand;
            }
        }
        ++next_int;
    }
}

/* Public function returning the n-th term of the sequence (1‑indexed). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* guard against 0‑based call, though not required */
    ensure_cache(n);
    return seq_cache[n - 1];
}
