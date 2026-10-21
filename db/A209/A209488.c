/*
 * A209488  Number of 7-bead necklaces labeled with numbers -n..n allowing
 *          reversal, with sum zero and avoiding the patterns z z+1 z+2 and z
 *          z-1 z-2.
 * https://oeis.org/A209488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 34, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A209488: Number of 7‑bead necklaces with labels –n…n
   (reversal allowed), sum zero, avoiding the patterns
   z z+1 z+2 and z z–1 z–2.
   Empirical recurrence for n > 24 (OEIS entry):
   a(n) = 2*a(n-1) - a(n-3) - a(n-5) + a(n-6) - 2*a(n-7) + 2*a(n-8) + a(n-9)
          - a(n-13) - 2*a(n-14) + 2*a(n-15) - a(n-16) + a(n-17) + a(n-19)
          - 2*a(n-21) + a(n-22)
   Initial values a(1) … a(24) are given explicitly. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static seq_t *memo = NULL;      /* memoised values, indices 0.. */
static size_t memo_cap = 0;     /* capacity of memo array */
static size_t memo_last = 0;    /* highest pre‑computed index */

/* Ensure memo array can hold at least indices 0..need-1. */
static void ensure_capacity(size_t need)
{
    if (need <= memo_cap) return;
    size_t new_cap = memo_cap ? memo_cap * 2 : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure */
    memo = tmp;
    memo_cap = new_cap;
}

/* Initialise memo table with the first 24 terms (index 0 unused). */
static void init_memo(void)
{
    if (memo) return;
    static const seq_t init_vals[25] = {
        0ULL,                /* dummy for index 0 */
        15ULL, 458ULL, 3740ULL,    17771ULL,   60778ULL,
        168453ULL, 401634ULL, 857433ULL, 1679810ULL, 3074315ULL,
        5321674ULL, 8796771ULL, 13984178ULL, 21501971ULL, 32119660ULL,
        46785813ULL, 66648788ULL, 93089119ULL, 127741326ULL,
        172532039ULL, 229703658ULL, 301856427ULL,
        391974770ULL, 503474661ULL
    };
    memo_cap   = 25;
    memo_last  = 24;
    memo       = (seq_t *)malloc(memo_cap * sizeof(seq_t));
    if (!memo) abort();
    memcpy(memo, init_vals, memo_cap * sizeof(seq_t));
}

/* Compute a(n) with memoisation; n >= 1. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* only defined for n >= 1 */
    init_memo();

    /* Compute missing values up to n (if necessary). */
    if ((size_t)n + 1 > memo_cap) {
        ensure_capacity((size_t)n + 1);
    }

    for (size_t i = memo_last + 1; i <= (size_t)n; ++i) {
        /* Recurrence valid only for i > 24; all earlier values are pre‑loaded. */
        if (i <= 24) continue;

        /* Compute the sum of positive coefficients. */
        seq_t pos = 0;
        pos += memo[i - 1];                  /* 2*a(i-1) part 1 */
        pos += memo[i - 1];                  /* 2*a(i-1) part 2 */
        pos += memo[i - 6];                  /* + a(i-6) */
        pos += memo[i - 8]; pos += memo[i - 8]; /* + 2*a(i-8) */
        pos += memo[i - 9];                  /* + a(i-9) */
        pos += memo[i - 15]; pos += memo[i - 15]; /* + 2*a(i-15) */
        pos += memo[i - 17];                 /* + a(i-17) */
        pos += memo[i - 19];                 /* + a(i-19) */
        pos += memo[i - 22];                 /* + a(i-22) */

        /* Compute the sum of negative coefficients. */
        seq_t neg = 0;
        neg += memo[i - 3];                  /* - a(i-3) */
        neg += memo[i - 5];                  /* - a(i-5) */
        neg += memo[i - 7]; neg += memo[i - 7]; /* - 2*a(i-7) */
        neg += memo[i - 13];                 /* - a(i-13) */
        neg += memo[i - 14]; neg += memo[i - 14]; /* - 2*a(i-14) */
        neg += memo[i - 16];                 /* - a(i-16) */
        neg += memo[i - 21]; neg += memo[i - 21]; /* - 2*a(i-21) */

        memo[i] = pos - neg;                 /* final result (non‑negative) */
    }

    memo_last = (size_t)n > memo_last ? (size_t)n : memo_last;
    return memo[n];
}
