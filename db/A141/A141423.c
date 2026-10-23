/*
 * A141423  Conjectured to be complete list of numbers not of the form 3*x^2 +
 *          4*y^2 + triangular number.
 * https://oeis.org/A141423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   grow_seq_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th integer that cannot be written as 3*x^2 + 4*y^2 + T_k
   with non‑negative integers x,y,k (T_k = k(k+1)/2). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/grow_seq_idx.h"

/* ---------- global state ---------- */
static unsigned char *rep = NULL;   /* 0/1 flags, rep[n] true if representable */
static idx_t rep_size = 0;          /* allocated size of rep */
static idx_t *nonrep = NULL;        /* indices of non‑representable numbers */
static size_t nonrep_len = 0;       /* number of stored non‑rep values */
static size_t nonrep_cap = 0;       /* capacity of nonrep array */
static idx_t last_checked = 0;      /* largest n that has been examined */

/* ---------- rebuild representable array up to new_max ---------- */
static void rebuild_rep(idx_t new_max)
{
    /* allocate new array */
#ifdef __cplusplus
    unsigned char *new_rep = new unsigned char[(size_t)(new_max + 1)];
    memset(new_rep, 0, (size_t)(new_max + 1));
#else
    unsigned char *new_rep = (unsigned char *)calloc((size_t)(new_max + 1), 1);
    if (!new_rep) exit(EXIT_FAILURE);
#endif

    /* enumerate all representations 3*x^2 + 4*y^2 + T_k <= new_max */
    for (idx_t x = 0; ; ++x) {
        unsigned __int128 vx128 = (unsigned __int128)3 * x * x;
        if (vx128 > new_max) break;
        idx_t vx = (idx_t)vx128;
        for (idx_t y = 0; ; ++y) {
            unsigned __int128 base128 = (unsigned __int128)vx + (unsigned __int128)4 * y * y;
            if (base128 > new_max) break;
            idx_t base = (idx_t)base128;
            /* triangular loop */
            idx_t T = 0;   /* T_0 */
            idx_t k = 0;
            while (base <= new_max) {
                /* sum = base + T */
                idx_t sum = base + T;
                if (sum > new_max) break;
                new_rep[sum] = 1;
                k++;
                T += k;  /* T_{k} = T_{k-1} + k */
            }
        }
    }

    /* free old array and store new one */
    if (rep) {
#ifdef __cplusplus
        delete[] rep;
#else
        free(rep);
#endif
    }
    rep = new_rep;
    rep_size = new_max + 1;

    /* collect new non‑representable numbers */
    for (idx_t i = last_checked + 1; i <= new_max; ++i) {
        if (!rep[i]) {
            grow_seq_idx(&nonrep, &nonrep_cap, nonrep_len + 1);
            nonrep[nonrep_len++] = i;
        }
    }
    last_checked = new_max;
}

/* ---------- ensure we have at least n values in nonrep ---------- */
static void ensure_n_terms(idx_t n)
{
    idx_t target = rep_size > 0 ? (rep_size - 1) : 1;
    while (nonrep_len < n) {
        target <<= 1;
        rebuild_rep(target);
    }
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, return 0 for safety */
    ensure_n_terms(n);
    return (seq_t)nonrep[n - 1];
}
