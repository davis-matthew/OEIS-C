/*
 * A010067  Base 6 self or Colombian numbers (not of form k + sum of base 6
 *          digits of k).
 * https://oeis.org/A010067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sum_digits_base6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A010067 – Base 6 Self (Colombian) Numbers
   a(n) = the n‑th positive integer that is NOT of the form k + sum_digits_base6(k)  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/sum_digits_base6.h"

/* ------------------------------------------------------------------------- */
/*  Dynamic storage for the sieve                                          */
/* ------------------------------------------------------------------------- */

/* Boolean array: generated[i] == true  →  i has been written as k + s6(k)   */
static bool *generated   = NULL;    /* array of bool */
static size_t generated_cap = 0;     /* current maximum index + 1 */

/* Array of self numbers (values that never appear as k + s6(k))               */
static seq_t *ungenerated      = NULL;    /* array of seq_t */
static size_t ungenerated_cap   = 0;      /* current capacity */
static idx_t   ungenerated_cnt  = 0;      /* number of elements stored */

/* State for incremental sieve */
static idx_t processed_k = 0;  /* the next k to be processed */
static idx_t current_m   = 1;  /* the next natural number to test for selfness */

/* Ensure the generated array can index up to 'needed' (inclusive) */
static void ensure_generated_cap(size_t needed)
{
    if (needed <= generated_cap)
        return;
    size_t newcap = generated_cap ? generated_cap : 8;
    while (newcap <= needed)
        newcap <<= 1;
    bool *tmp = (bool *)realloc(generated, newcap * sizeof(bool));
    if (!tmp) abort();
    /* initialise new slots to false */
    for (size_t i = generated_cap; i < newcap; ++i)
        tmp[i] = false;
    generated = tmp;
    generated_cap = newcap;
}

/* Ensure the ungenerated array has capacity for 'needed' elements           */
static void ensure_ungenerated_cap(size_t needed)
{
    if (needed <= ungenerated_cap)
        return;
    size_t newcap = ungenerated_cap ? ungenerated_cap : 8;
    while (newcap <= needed)
        newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(ungenerated, newcap * sizeof(seq_t));
    if (!tmp) abort();
    ungenerated = tmp;
    ungenerated_cap = newcap;
}

/* ------------------------------------------------------------------------- */
/*  Public function                                                        */
/* ------------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* nothing is required for n == 0 – the offset is 1 */
    if (n == 0)
        return 0;

    /* Generate self numbers until we have at least n of them. */
    while (ungenerated_cnt < n) {
        /* First, mark all k + s6(k) for k up to current_m.  */
        while (processed_k <= current_m) {
            idx_t k = processed_k;
            seq_t f = (seq_t)k + sum_digits_base6(k);

            /* f must fit into a size_t index for the array.  For
               our density and the required n this is true. */
            if ((size_t)f >= SIZE_MAX)
                abort();

            ensure_generated_cap((size_t)f + 1);
            generated[(size_t)f] = true;
            ++processed_k;
        }

        /* Now test whether current_m is a self number. */
        if (!(current_m < (idx_t)generated_cap && generated[(size_t)current_m])) {
            ensure_ungenerated_cap(ungenerated_cnt + 1);
            ungenerated[ungenerated_cnt] = (seq_t)current_m;
            ++ungenerated_cnt;
        }
        ++current_m;
    }

    return ungenerated[n - 1];
}
