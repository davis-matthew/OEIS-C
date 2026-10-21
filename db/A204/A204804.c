/*
 * A204804  Number of free tree-like convex polyominoes with n cells.
 * https://oeis.org/A204804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A204804: Number of free tree‑like convex polyominoes with n cells.
   Formula:
        a(n) = 2^(n-1) + 2^((n-1)/2) - b(n)
   where b(n) has the following linear recurrence (order 11):
        b(n) = 4 b(n‑1) – 2 b(n‑2) – 10 b(n‑3) + 14 b(n‑4)
               – 2 b(n‑5) – 8 b(n‑6) + 14 b(n‑7) – 13 b(n‑8)
               – 2 b(n‑9) + 10 b(n‑10) – 4 b(n‑11)
   Initial values (b(1)…b(11)):
        1, 2, 4, 6, 10, 15, 23, 32, 45, 60, 80
*/

#include <stdlib.h>
#include "../utils/pow2.h"

/* --- b(n) cache -------------------------------------------------------- */
static seq_t *b_cache   = NULL; /* index 1… will store b(n) */
static size_t b_cap     = 0;    /* allocated size (≥ b_len)          */
static idx_t  b_len     = 0;    /* highest n with b_cache[n] set     */

/* ensure capacity for indices up to 'needed' (inclusive) */
static void ensure_b_capacity(idx_t needed)
{
    if (needed < b_cap)
        return;
    size_t new_cap = needed + 1;                /* capacity for indices 0…needed */
    if (new_cap < b_cap * 2)
        new_cap = b_cap * 2;
    seq_t *tmp = (seq_t *)realloc(b_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    b_cache = tmp;
    b_cap   = new_cap;
}

/* initialise cache with first 11 non‑zero values of b(n) (b(1)…b(11)) */
static void init_b_cache(void)
{
    if (b_len > 0)                 /* already initialised */
        return;
    ensure_b_capacity(12);         /* need indices 0…12 */
    /* indices 0 unused; values 1…11 are the initial terms */
    b_cache[1]  = (seq_t)1;
    b_cache[2]  = (seq_t)2;
    b_cache[3]  = (seq_t)4;
    b_cache[4]  = (seq_t)6;
    b_cache[5]  = (seq_t)10;
    b_cache[6]  = (seq_t)15;
    b_cache[7]  = (seq_t)23;
    b_cache[8]  = (seq_t)32;
    b_cache[9]  = (seq_t)45;
    b_cache[10] = (seq_t)60;
    b_cache[11] = (seq_t)80;
    b_len = 11;
}

/* extend cache up to n (inclusive) using the recurrence */
static void compute_b_up_to(idx_t n)
{
    init_b_cache();
    if (n <= b_len)
        return;

    for (idx_t i = b_len + 1; i <= n; ++i) {
        /* recurrence requires terms up to i‑11, which exist because i ≥ 12 */
        seq_t pos = 4 * b_cache[i-1] + 14 * b_cache[i-4]
                  + 14 * b_cache[i-7] + 10 * b_cache[i-10];
        seq_t neg = 2 * b_cache[i-2] + 10 * b_cache[i-3]
                  + 2 * b_cache[i-5] +  8 * b_cache[i-6]
                  +13 * b_cache[i-8] +  2 * b_cache[i-9]
                  + 4 * b_cache[i-11];
        b_cache[i] = pos - neg;
    }
    b_len = n;
}

/* ---------------------------------------------------------------------- */
/*  Public entry point                                                     */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* compute b(n) if necessary */
    if (n >= b_cap)
        ensure_b_capacity(n);
    if (b_len < n)
        compute_b_up_to(n);

    /* compute the two powers of two */
    seq_t p1 = pow2(n - 1);           /* 2^(n-1)           */
    seq_t p2 = pow2((n - 1) / 2);     /* 2^((n-1)/2)       */

    /* a(n) = p1 + p2 - b(n) */
    return p1 + p2 - b_cache[n];
}
