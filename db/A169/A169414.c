/*
 * A169414  Number of reduced words of length n in Coxeter group on 17
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = num_coeff(n) + 15*∑_{k=1}^{31} a(n-k) – 120*a(n-32) where
 *            num_coeff(0)=1, num_coeff(1)…num_coeff(31)=2, num_coeff(32)=1,
 *            num_coeff(k)=0 for k>32
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   num_coeff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses memoised iteration and is valid C11 and C++17. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_coeff.h"

/* global memoisation array */
static seq_t *memo = NULL;       /* pointer to the computed values */
static size_t memo_cap = 0;      /* allocated capacity (in elements) */
static size_t memo_len = 0;      /* number of values currently stored */

/* enlarge the memo array to at least ‘need’ elements */
static void ensure_capacity(size_t need)
{
    if (need <= memo_cap) return;
    size_t new_cap = memo_cap ? memo_cap << 1 : 64;
    while (new_cap < need) new_cap <<= 1;
    /* use explicit casts so the code works in C++ even when seq_t
       is a non‑trivial class type (the constructors are trivial for
       types used in the OEIS test harness). */
    seq_t *new_mem = memo ? (seq_t*)realloc(memo, new_cap * sizeof(seq_t))
                          : (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_mem) abort();   /* allocation failed */
    memo = new_mem;
    memo_cap = new_cap;
}

/* the sequence function */
seq_t a(idx_t n)
{
    /* initialise the first term if this is the first call */
    if (memo_len == 0) {
        ensure_capacity(1);
        memo[0] = num_coeff(0);   /* a(0) = 1 */
        memo_len = 1;
    }

    /* compute values up to n */
    while (memo_len <= n) {
        idx_t i = (idx_t)memo_len;   /* current index to compute */
        seq_t sum = num_coeff(i);

        /* 15 * sum_{k=1}^{31} a(i-k) */
        for (idx_t k = 1; k <= 31; ++k) {
            if (i >= k) {
                sum += memo[i - k] * 15;
            }
        }

        /* – 120 * a(i-32) if applicable */
        if (i >= 32) {
            sum -= memo[i - 32] * 120;
        }

        ensure_capacity(memo_len + 1);
        memo[memo_len++] = sum;
    }

    return memo[n];
}
