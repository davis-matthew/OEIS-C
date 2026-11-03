/*
 * A085954  a(1) = 4, a(n+1) = the smallest multiple of composite(n+1)
 *          beginning with a(n).
 * https://oeis.org/A085954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 4, * a(n+1) = the smallest multiple of composite(n+1)
 *            beginning with a(n)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 17 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper utilities from the build environment.                           */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Find the smallest multiple of `cmp` whose decimal representation begins
   with the digits of `prev`.  We successively search over
   suffixes of increasing length (10⁰,10¹,10²,…). */
static seq_t smallest_multiple_with_prefix(seq_t prev, seq_t cmp)
{
    seq_t pow10 = 1;                 /* 10ᵗ */
    while (true) {
        seq_t L = prev * pow10;                  /* lower bound of numbers with same prefix        */
        seq_t U = (prev + 1) * pow10;            /* upper bound (exclusive)                       */

        /* smallest k with cmp*k >= L  */
        seq_t k_start = (L + cmp - 1) / cmp;
        seq_t m = k_start * cmp;

        if (m < U)                /* found a multiple that shares the prefix */
            return m;

        pow10 = pow10 * 10;        /* next suffix length                         */
    }
}

/* Check if a number n (1‑based) is composite.  We keep a growing list
   of composite numbers in ascending order.  Composite(1)=4, Composite(2)=6, ... */
static uint64_t *comps     = NULL;   /* 1‑based: comps[1] = 4, comps[2] = 6, ...          */
static size_t  comp_len   = 0;        /* amount currently stored */
static size_t  comp_cap   = 0;        /* allocated capacity */
static uint64_t comp_next = 4;        /* next integer to test */

static void ensure_composites(size_t need)          /* need >= comps[need-1] exists */
{
    if (comp_len >= need) return;

    if (!comps) {
        comp_cap = 1024;
        comps = (uint64_t*)malloc(comp_cap * sizeof(uint64_t));
    }

    while (comp_len < need) {
        if (!is_prime((idx_t)comp_next)) {
            if (comp_len >= comp_cap) {
                comp_cap <<= 1;
                comps = (uint64_t*)realloc(comps, comp_cap * sizeof(uint64_t));
            }
            comps[comp_len++] = comp_next;
        }
        comp_next++;
    }
}

/* Memoized computation of a(n).  We store values 1‑based in a dynamic array. */
seq_t a(idx_t n)
{
    if (n == 1)                     /* base value explicitly defined               */
        return (seq_t)4;

    /* Memoization array --------------------------------------------------- */
    static seq_t *memo      = NULL;   /* memo[1] .. memo[n]  */
    static size_t memo_len  = 0;       /* current allocated size (index 0 unused)     */

    if ((size_t)n >= memo_len) {      /* need more space */
        size_t new_cap = (size_t)n + 2;   /* one beyond n for 1‑based index */
        seq_t *new_arr = mem_resize(seq_t, memo, new_cap);
        if (!memo) memo = new_arr; else memo = new_arr;
        for (size_t i = memo_len; i < new_cap; ++i)
            memo[i] = 0;
        memo_len = new_cap;
    }

    /* Base case stored if not already present */
    if (memo[1] == 0) memo[1] = (seq_t)4;

    /* Compute every term up to n, skipping cached ones */
    for (size_t i = 2; i <= (size_t)n; ++i) {
        if (memo[i] != 0) continue;    /* already computed */

        seq_t prev = memo[i - 1];      /* a(i-1) */
        size_t comp_idx = i;           /* composite(i) is needed */
        ensure_composites(comp_idx);
        seq_t cmp = (seq_t)comps[comp_idx - 1];   /* comps is 1‑based */

        memo[i] = smallest_multiple_with_prefix(prev, cmp);
    }

    return memo[(size_t)n];
}
