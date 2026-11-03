/*
 * A212718  Denominator of Sum_{k=1..n} 1/sigma(k).
 * https://oeis.org/A212718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of \sum_{k=1..n} 1/sigma(k)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-03)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

/* Ensure an array of seq_t is at least new_cap elements long,
   zero‑filling any newly allocated part. */
static void extend_seqt(seq_t **arr, idx_t *cur_cap, idx_t new_cap)
{
    if (new_cap <= *cur_cap)
        return;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp)
        exit(1);                                   /* allocation failure */
    for (idx_t i = *cur_cap; i < new_cap; ++i)
        tmp[i] = 0;
    *arr = tmp;
    *cur_cap = new_cap;
}

/* Ensure an array of uint64_t is at least new_cap elements long,
   zero‑filling any newly allocated part. */
static void extend_uint64(uint64_t **arr, idx_t *cur_cap, idx_t new_cap)
{
    if (new_cap <= *cur_cap)
        return;
    uint64_t *tmp = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
    if (!tmp)
        exit(1);                                   /* allocation failure */
    for (idx_t i = *cur_cap; i < new_cap; ++i)
        tmp[i] = 0;
    *arr = tmp;
    *cur_cap = new_cap;
}

/* Recompute all sigma values for indices 1..n.
   This simply runs the classic divisor‑sum sieve. */
static void recompute_sigma(uint64_t *sigma, idx_t n)
{
    /* Zero the working part */
    for (idx_t i = 1; i <= n; ++i)
        sigma[i] = 0;

    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i)
            sigma[j] += i;
    }
}

seq_t a(idx_t n)
{
    /* Static state ---------------------------------------------------- */
    static idx_t computed_max = 0;          /* largest index computed so far   */
    static idx_t sigma_size   = 0;          /* current allocated size of sigma */
    static uint64_t *sigma     = NULL;      /* sigma[1..sigma_size-1]          */
    static idx_t denom_size   = 0;          /* current allocated size of denom  */
    static seq_t *denom       = NULL;       /* denom[1..denom_size-1]          */
    static seq_t curr_num     = 0;          /* numerator of current sum        */
    static seq_t curr_den     = 1;          /* denominator of current sum      */

    /* make sure our arrays are large enough (index 0 is unused) */
    idx_t new_cap = n + 1;
    extend_uint64(&sigma,   &sigma_size, new_cap);
    extend_seqt(&denom,     &denom_size, new_cap);

    /* if the requested n is already prepared, return it */
    if (n <= computed_max)
        return denom[n];

    /* Recreate divisor sums up to the new maximum n.
       This guarantees that sigma[1..n] are correct. */
    recompute_sigma(sigma, n);

    /* Append new terms for k = computed_max+1 .. n */
    for (idx_t k = computed_max + 1; k <= n; ++k) {
        seq_t sigma_k = (seq_t)sigma[k];

        /* new_num = curr_num * sigma_k + curr_den          */
        seq_t new_num = curr_num * sigma_k + curr_den;

        /* new_den = curr_den * sigma_k                     */
        seq_t new_den = curr_den * sigma_k;

        /* reduce fraction */
        seq_t g = gcd(new_num, new_den);
        curr_num = new_num / g;
        curr_den = new_den / g;

        /* store the reduced denominator */
        denom[k] = curr_den;
    }

    computed_max = n;
    return denom[n];
}
