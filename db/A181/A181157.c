/*
 * A181157  a(n) is the greatest integer such that the occurrence rate of
 *          integers k and above in {a(1), ..., a(n)} <= 1/k for all positive
 *          integers k.
 * https://oeis.org/A181157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the greatest integer m such that for every k ≥ 1, *
 *            count_{i ≤ n} (a(i) ≥ k) ≤ n / k. * The implementation lazily
 *            builds the sequence up to the requested n, * maintaining a
 *            suffix count array. Each new term is the largest m * that keeps
 *            all inequalities satisfied
 * Range:     u8 n <= 405, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* global state for memoization */
static size_t seq_cap = 0;       /* capacity of allocated arrays */
static idx_t   max_n   = 0;      /* highest n computed so far */
static seq_t *seq_arr  = NULL;   /* 1‑based sequence values */
static idx_t *suffix   = NULL;   /* suffix[k] = #terms among a(1)…a(n‑1) ≥ k */

/* ensure arrays are large enough for index n */
static void
ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;      /* 0…n inclusive */
    if (needed <= seq_cap)
        return;

    size_t new_cap = needed;
    seq_arr  = (seq_t *)realloc(seq_arr,  new_cap * sizeof(*seq_arr));
    suffix   = (idx_t *)realloc(suffix,   new_cap * sizeof(*suffix));

    /* initialise any newly added entries of suffix to zero */
    for (size_t i = seq_cap; i < new_cap; ++i)
        suffix[i] = 0;

    seq_cap = new_cap;
}

/* extend precomputation up to at least n */
static void
extend_up_to(idx_t n)
{
    if (n <= max_n)
        return;

    ensure_capacity(n);

    for (idx_t cur = max_n + 1; cur <= n; ++cur) {
        /* determine a(cur) */
        seq_t a_cur = cur;          /* assume maximum possible */
        for (idx_t k = 2; k <= cur; ++k) {
            idx_t bound = cur / k;                /* floor(cur/k) */
            if (suffix[(size_t)k] > bound - 1) {   /* violation if a_cur ≥ k */
                a_cur = k - 1;                     /* maximal allowed m */
                break;
            }
        }
        seq_arr[cur] = a_cur;

        /* update suffix counts for the new term */
        for (idx_t k = 2; k <= a_cur; ++k)
            suffix[(size_t)k] += 1;
    }

    max_n = n;
}

seq_t a(idx_t n)
{
    extend_up_to(n);
    return seq_arr[n];
}
