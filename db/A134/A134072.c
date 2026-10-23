/*
 * A134072  Concatenation of A000204 Lucas numbers (beginning at 1) in reverse
 *          order.
 * https://oeis.org/A134072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of L_n, L_{n-1}, … , L_1 in decimal;
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where L_0 = 2, L_1 = 1 and L_k = L_{k-1} + L_{k-2} for k ≥ 2.  */
/* This function supports any order of calls and memoises the Lucas */
/* numbers up to the largest n requested.  */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

/* ---------- Lucas‑number cache ---------- */
static seq_t *lucas_cache = NULL;  /* lucas_cache[i] = L_i */
static size_t lucas_cap  = 0;      /* allocated capacity        */
static idx_t  lucas_len  = 0;      /* next free index           */

/* Ensure lucas_cache contains Lucas numbers up to index n (inclusive). */
static void ensure_lucas(idx_t n)
{
    if (!lucas_cache) {
        /* Initialise with L_0 = 2, L_1 = 1. */
        lucas_cap = 4;
        lucas_len = 2;
        lucas_cache = (seq_t *)malloc(lucas_cap * sizeof(seq_t));
        lucas_cache[0] = (seq_t)2;
        lucas_cache[1] = (seq_t)1;
    }
    while (lucas_len <= n) {
        if (lucas_len >= lucas_cap) {
            lucas_cap <<= 1;
            lucas_cache = (seq_t *)realloc(lucas_cache,
                                           lucas_cap * sizeof(seq_t));
        }
        lucas_cache[lucas_len] = lucas_cache[lucas_len - 1] +
                                 lucas_cache[lucas_len - 2];
        ++lucas_len;
    }
}

/* ---------- Sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* defined for safety, offset is 1 */
    ensure_lucas(n);               /* have L_0 … L_n ready */

    seq_t result = 0;
    for (idx_t i = n; i > 0; --i) {
        seq_t l   = lucas_cache[i];          /* current Lucas number */
        int    d  = digit_len(l);            /* decimal digits of l   */
        seq_t pow = pow10((idx_t)d);          /* 10^d                  */
        result = result * pow + l;           /* append l to result    */
    }
    return result;
}
