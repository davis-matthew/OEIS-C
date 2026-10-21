/*
 * A066200  a(1) = 1; for n > 1, a(n) = a(n-1)-(n+2) if this is positive and
 *          has not already appeared, otherwise a(n) = a(n-1)+(n+2).
 * https://oeis.org/A066200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 115, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_cap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1;
   For n > 1:
        a(n) = a(n-1) – (n+2)  if the result is positive and not already seen,
        otherwise a(n) = a(n-1) + (n+2).                           */
#include <stdlib.h>   /* malloc, realloc */
#include "../utils/grow_cap.h"

/* The function is opaque to the build system: seq_t and idx_t are defined
   elsewhere. The implementation uses only integer types and the standard
   library.                                               */

static seq_t *a_vals   = NULL;   /* array of A066200 values (1‑based)   */
static size_t a_len    = 0;      /* current length (elements allocated) */
static size_t a_cap    = 0;      /* capacity of a_vals                  */

static seq_t *seen     = NULL;   /* array of values already produced   */
static size_t seen_len = 0;      /* number of values seen              */
static size_t seen_cap = 0;      /* capacity of seen                   */

static int initialized = 0;      /* flag for one‑time initialization   */

/* Initialise the static tables on the first call. */
static void init_tables(void)
{
    /* allocate a_vals: index 0 unused, a_vals[1] = a(1) = 1 */
    grow_cap(&a_cap, &a_vals, 3);      /* small start capacity */
    a_vals[0] = 0;                     /* dummy, never used          */
    a_vals[1] = 1;
    a_len = 2;                         /* entries stored: {0,1}     */

    /* allocate seen: holds values already output */
    grow_cap(&seen_cap, &seen, 3);
    seen[0] = 1;
    seen_len = 1;

    initialized = 1;
}

/* Test whether 'x' is already in the seen array. */
static int seen_contains(seq_t x)
{
    for (size_t i = 0; i < seen_len; ++i)
        if (seen[i] == x) return 1;
    return 0;
}

/* Main function implementing the sequence. */
seq_t a(idx_t n)
{
    if (!initialized) init_tables();

    /* extend the tables up to index n (inclusive) */
    while (a_len <= (size_t)n)
    {
        size_t idx   = a_len;          /* next index, 1‑based   */
        seq_t prev   = a_vals[idx - 1];
        seq_t delta  = (seq_t)(idx + 2);/* (n+2) as seq_t      */

        /* try subtraction only if it stays positive */
        if (prev > delta)
        {
            seq_t cand = prev - delta;
            if (!seen_contains(cand))
            {
                /* append cand */
                grow_cap(&a_cap, &a_vals, a_len + 1);
                a_vals[a_len] = cand;
                ++a_len;

                grow_cap(&seen_cap, &seen, seen_len + 1);
                seen[seen_len] = cand;
                ++seen_len;
                continue;   /* next term */
            }
        }

        /* fallback to addition */
        seq_t val = prev + delta;
        grow_cap(&a_cap, &a_vals, a_len + 1);
        a_vals[a_len] = val;
        ++a_len;

        grow_cap(&seen_cap, &seen, seen_len + 1);
        seen[seen_len] = val;
        ++seen_len;
    }

    return a_vals[(size_t)n];
}
