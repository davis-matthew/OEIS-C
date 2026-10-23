/*
 * A169482  Number of reduced words of length n in Coxeter group on 37
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^33 = I.
 * https://oeis.org/A169482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   alloc_seq_array, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A169482  –  Number of reduced words of length n in the Coxeter group on
 * 37 generators with (S_i)^2 = (S_i S_j)^33 = I.
 *
 *  Recurrence:
 *    a(0) = 1
 *    a(n) = 2 + 35 * Σ_{k=0}^{n-1} a(k)                     for 1 ≤ n ≤ 32
 *    a(n) = 35 * Σ_{k=n-32}^{n-1} a(k) – 630 * a(n-33)       for n ≥ 33
 *
 *  The implementation memoises the sequence so that each term is computed
 *  only once, allowing calls to a(idx_t n) in any order.
 */

#include <stddef.h>                     /* size_t */
#include "../utils/alloc_seq_array.h"   /* seq_t *alloc_seq_array(size_t n) */
#include "../utils/seq_free.h"          /* void seq_free(seq_t *p) */

/* Static cache of already–computed terms */
static seq_t *seq_tbl = NULL;           /* a[0..]                           */
static seq_t *cum_tbl = NULL;           /* cumulative sums: cum_tbl[i]=∑_{k< i} a(k)  */
static size_t tbl_cap = 0;              /* current capacity of seq_tbl      */
static idx_t  last    = 0;              /* largest computed index           */
static int    init    = 0;              /* whether the cache has been init. */

/* Ensure the cache can store terms up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    size_t need = (size_t)n + 1;        /* number of a[0..n] needed */
    if (need <= tbl_cap) return;       /* already sufficient */

    /* Allocate new buffers with the required capacity */
    seq_t *new_seq = alloc_seq_array(need);
    seq_t *new_cum = alloc_seq_array(need + 1);

    /* Copy existing data if any */
    if (seq_tbl && cum_tbl) {
        size_t old_cap = tbl_cap;
        /* seq_tbl has old_cap elements, cum_tbl has old_cap+1 */
        for (size_t i = 0; i < old_cap; ++i) new_seq[i] = seq_tbl[i];
        for (size_t i = 0; i < old_cap + 1; ++i) new_cum[i] = cum_tbl[i];

        /* Free old buffers */
        seq_free(seq_tbl);
        seq_free(cum_tbl);
    }

    /* The new cumulative table keeps its existing prefix sums */
    /* (new_cum[0] already copied from old table, or uninitialized if first) */

    /* Install the new buffers */
    seq_tbl = new_seq;
    cum_tbl = new_cum;
    tbl_cap = need;
}

/* Compute a(n) and return the value */
seq_t a(idx_t n)
{
    ensure_capacity(n);

    /* Initialise the cache on first use */
    if (!init) {
        seq_tbl[0] = 1;      /* a(0) = 1 */
        cum_tbl[0] = 0;      /* cum[0] = 0 */
        cum_tbl[1] = seq_tbl[0];  /* cum[1] = a(0) */
        last = 0;
        init = 1;
    }

    /* Extend the cache up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 32) {
            /* a(i) = 2 + 35 * Σ_{k=0}^{i-1} a(k) = 2 + 35 * cum_tbl[i] */
            val = ((seq_t)2) + ((seq_t)35) * cum_tbl[i];
        } else { /* i >= 33 */
            /* a(i) = 35 * Σ_{k=i-32}^{i-1} a(k) – 630 * a(i-33) */
            seq_t sum_prev32 = cum_tbl[i] - cum_tbl[i - 32];
            val = ((seq_t)35) * sum_prev32 - ((seq_t)630) * seq_tbl[i - 33];
        }
        seq_tbl[i] = val;
        cum_tbl[i + 1] = cum_tbl[i] + val;
    }

    last = n;
    return seq_tbl[n];
}
