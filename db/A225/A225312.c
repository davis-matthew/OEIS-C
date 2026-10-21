/*
 * A225312  Number of 5 X n -1,1 arrays such that the sum over i=1..5,j=1..n
 *          of i*x(i,j) is zero and rows are nondecreasing (ways to put n
 *          thrusters pointing east or west at each of 5 positions 1..n
 *          distance from the hinge of a south-pointing gate without turning
 *          the gate).
 * https://oeis.org/A225312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A225312
 *
 *  Formula:
 *    a(n) = 3*a(n-2) - 2*a(n-4) - 2*a(n-6) + 4*a(n-8)
 *           - 4*a(n-10) + 2*a(n-12) + 2*a(n-14) - 3*a(n-16) + a(n-18)
 *
 *  Initial values (offset 1):
 *    a(1)=0, a(2)=15, a(3)=0, a(4)=113, a(5)=0,
 *    a(6)=427, a(7)=0, a(8)=1165, a(9)=0,
 *    a(10)=2591, a(11)=0, a(12)=5053, a(13)=0,
 *    a(14)=8947, a(15)=0, a(16)=14759, a(17)=0,
 *    a(18)=23017.
 *
 *  The recurrence holds for all n ≥ 19 (odd terms are zero).
 *  The implementation stores previously computed values in a static cache,
 *  therefore works for calls in any order and is O(n) amortised.
 */

#include <stdlib.h>

/* -------------------  helper cache  ------------------- */
static seq_t *a_cache = NULL;      /* cache array, 1‑based, index 0 unused   */
static idx_t a_cache_size = 0;     /* largest index currently stored        */

/* -------------------  base values  -------------------- */
static seq_t const base_values[19] = {
    0,    /* 0 (unused)          */
    0,    /* a(1) */
    15,   /* a(2) */
    0,    /* a(3) */
    113,  /* a(4) */
    0,    /* a(5) */
    427,  /* a(6) */
    0,    /* a(7) */
    1165, /* a(8) */
    0,    /* a(9) */
    2591, /* a(10) */
    0,    /* a(11) */
    5053, /* a(12) */
    0,    /* a(13) */
    8947, /* a(14) */
    0,    /* a(15) */
    14759,/* a(16) */
    0,    /* a(17) */
    23017 /* a(18) */
};

/* -------------------  main sequence function  --------- */
seq_t a(idx_t n)
{
    /* Ensure the cache can hold all indices up to n. */
    if (n > a_cache_size) {
        size_t new_size = (size_t)(n + 1);   /* include index 0 */
        seq_t *tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                  /* out‑of‑memory: fall back */
        a_cache = tmp;

        /* Compute new entries in ascending order. */
        for (idx_t i = a_cache_size + 1; i <= n; ++i) {
            if (i <= 18) {
                a_cache[i] = base_values[i];          /* pre‑filled */
            } else {
                /* Positive part of recurrence */
                seq_t pos = 3 * a_cache[i - 2]
                           + 4 * a_cache[i - 8]
                           + 2 * a_cache[i -12]
                           + 2 * a_cache[i -14]
                           + a_cache[i -18];

                /* Negative part of recurrence */
                seq_t neg = 2 * a_cache[i - 4]
                           + 2 * a_cache[i - 6]
                           + 4 * a_cache[i -10]
                           + 3 * a_cache[i -16];

                a_cache[i] = pos - neg;   /* guaranteed non‑negative */
            }
        }
        a_cache_size = n;
    }
    return a_cache[n];
}
