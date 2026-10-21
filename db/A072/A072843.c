/*
 * A072843  O'Halloran numbers: even integers which cannot be the surface area
 *          of a cuboid with integer-length sides.
 * https://oeis.org/A072843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = the n‑th even integer that cannot be expressed as 2
 *            (ab+bc+ca) for positive integers a,b,c
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *olist   = NULL;   /* cache of O'Halloran numbers      */
static size_t  olen   = 0;       /* number of stored terms           */
static idx_t   bound  = 0;       /* largest surface area considered */

/* Build the list of O'Halloran numbers up to 'newbound'. */
static void rebuild_list(idx_t newbound)
{
    /* Flag representable surface areas up to newbound. */
    uint8_t *repr = (uint8_t *)calloc((size_t)newbound + 1, 1);
    if (!repr)
        abort();

    for (idx_t a = 1; ; ++a) {
        idx_t min_area = 2*a*a + 4*a;   /* a=b gives the smallest area for this a */
        if (min_area > newbound)
            break;

        for (idx_t b = a; ; ++b) {
            idx_t area = 2*a*b + 2*(a + b);   /* c = 1          */
            if (area > newbound)
                break;

            idx_t step = 2*(a + b);           /* increment for each c */
            for (idx_t s = area; s <= newbound; s += step)
                repr[s] = 1;
        }
    }

    /* Count how many even numbers are missing. */
    size_t est = 0;
    for (idx_t t = 8; t <= newbound; t += 2)
        if (!repr[t])
            ++est;

    /* Allocate temporary array for the missing numbers. */
#ifdef __cplusplus
    seq_t *tmp = est ? new seq_t[est] : nullptr;
#else
    seq_t *tmp = est ? (seq_t *)calloc(est, sizeof(seq_t)) : NULL;
#endif

    size_t idx = 0;
    for (idx_t t = 8; t <= newbound; t += 2)
        if (!repr[t]) {
#ifndef __cplusplus
            tmp[idx] = (seq_t)t;   /* integer types */
#else
            tmp[idx] = (seq_t)t;   /* big‑int types */
#endif
            ++idx;
        }

    free(repr);

    /* Replace the old cache. */
#ifdef __cplusplus
    delete[] olist;
#else
    free(olist);
#endif
    olist = tmp;
    olen  = est;
    bound = newbound;
}

/* External linkage: required sequence function. */
seq_t a(idx_t n)
{
    /* Initialise or extend the cache as necessary. */
    if (olist == NULL || olen <= n) {
        idx_t new_bound = bound ? bound : 200ULL;
        rebuild_list(new_bound);
        while (olen <= n) {
            new_bound *= 2ULL;
            rebuild_list(new_bound);
        }
    }

    return olist[n];
}
