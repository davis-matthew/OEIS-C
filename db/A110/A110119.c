/*
 * A110119  Self-inverse integer permutation induced by Beatty sequences for x
 *          and (x+1)/(2*sqrt(2)) with x=sqrt(2)+sqrt(3).
 * https://oeis.org/A110119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   self‑inverse permutation induced by the complementary Beatty
 *            sequences for α = √2 + √3 and β = (α + 1)/(2√2). For each
 *            integer k ≥ 1 let x_k = ⌊kα⌋ and y_k = ⌊kβ⌋. Define a(x_k) = y_k
 *            and a(y_k) = x_k. Thus a is an involution (a(a(n)) = n) for all
 *            n ≥ 1
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static seq_t   *perm   = NULL;      /* permutation table (index → value) */
static size_t  perm_cap = 0;        /* allocated capacity of perm array   */
static idx_t   last_k   = 0;        /* last Beatty index processed        */
static long double alpha, beta;    /* Beatty constants                   */
static bool    initialized = false;

/* Expand perm so that indices up to |req|‑1 are valid. */
static void ensure_capacity(size_t req)
{
    if (perm_cap < req) {
        seq_t *tmp = (seq_t*)realloc(perm, req * sizeof(seq_t));
        if (!tmp) abort();          /* allocation error */
        /* Zero the new part of the array. */
        if (req > perm_cap)
            memset(tmp + perm_cap, 0, (req - perm_cap) * sizeof(seq_t));
        perm = tmp;
        perm_cap = req;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* One‑time initialisation of constants and table. */
    if (!initialized) {
        alpha = sqrtl(2.0L) + sqrtl(3.0L);
        beta  = (alpha + 1.0L) / (2.0L * sqrtl(2.0L));

        /* Initial small capacity and zero the table. */
        ensure_capacity(16);
        memset(perm, 0, perm_cap * sizeof(seq_t));

        last_k = 0;
        initialized = true;
    }

    /* Generate Beatty pairs until index n has an entry. */
    while (perm[n] == 0) {               /* n is still unmapped */
        idx_t k = ++last_k;               /* next Beatty index    */
        idx_t x = (idx_t)floorl((long double)k * alpha);
        idx_t y = (idx_t)floorl((long double)k * beta);

        /* Make sure the array can hold both indices. */
        if (x >= perm_cap) ensure_capacity(x + 1);
        if (y >= perm_cap) ensure_capacity(y + 1);

        /* Store the involutive mapping. */
        perm[x] = (seq_t)y;
        perm[y] = (seq_t)x;
    }

    return perm[n];
}
