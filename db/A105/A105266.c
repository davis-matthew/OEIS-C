/*
 * A105266  a(1)=1 and, for n>1, a(n) is the smallest integer greater than
 *          a(n-1) such that no three terms x,y,z of the sequence, with x<y<z,
 *          satisfy z-y=y-x+1.
 * https://oeis.org/A105266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A105266
    a(1)=1; for n>1 the smallest integer greater than a(n‑1) that
    makes the sequence avoid triples x<y<z with z‑y = y‑x+1.
    Equivalently, a(n) is the n‑th positive integer whose base‑3
    representation contains at most one digit '1', and if the number
    ends with zeros those zeros are preceded by a '1'.  */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* helper to allocate an array of seq_t that works in both C and C++ */
static inline seq_t *seq_malloc(idx_t n)
{
#ifdef __cplusplus
    return mem_new(seq_t, n);
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

static inline seq_t *seq_realloc(seq_t *p, idx_t n)
{
#ifdef __cplusplus
    return mem_resize(seq_t, p, n);
#else
    return (seq_t *)realloc(p, n * sizeof(seq_t));
#endif
}

static bool is_valid(seq_t x)
{
    if (x == 0U) return false;          /* no zero in sequence */

    seq_t tmp = x;
    /* first (least significant) digit */
    unsigned first_digit = (unsigned)(tmp % (seq_t)3);

    bool found_one  = false;
    bool lower_nonzero = false;

    while (tmp != (seq_t)0)
    {
        unsigned d = (unsigned)(tmp % (seq_t)3);

        if (d)                      /* d == 1 or 2 */
        {
            if (!found_one)
            {
                if (d == 1)
                {
                    /* a 1 cannot have lower non‑zero digits */
                    if (lower_nonzero) return false;
                    found_one = true;
                }
            }
            else
            {
                /* more than one '1' would create a lower non‑zero after a 1 */
                if (d == 1) return false;
            }
            lower_nonzero = true;
        }
        /* d == 0 : nothing changes except that lower_nonzero may be true
           if we have seen a non‑zero digit already */
        tmp /= (seq_t)3;
    }

    /* if the number ends with zeros but contains no '1' → invalid */
    if (first_digit == 0 && !found_one) return false;

    return true;
}

static seq_t *seq   = NULL;          /* dynamic array of terms */
static idx_t  sz    = 0;             /* number of terms stored */
static idx_t  cap   = 0;             /* current allocated capacity */

seq_t a(idx_t n)
{
    if (n == 0) return 0U;           /* undefined for 0, but avoid UB */

    /* Ensure we have at least n terms */
    while (sz < n)
    {
        /* grow capacity when needed */
        if (cap == 0) cap = 16;
        while (cap < n) cap <<= 1;    /* double until sufficient */

        if (seq == NULL)
            seq = seq_malloc(cap);
        else
            seq = seq_realloc(seq, cap);

        /* generate missing terms */
        seq_t cand = (sz == 0) ? 1U : seq[sz-1] + 1U;
        while (sz < n)
        {
            if (is_valid(cand))
                seq[sz++] = cand;
            ++cand;
        }
    }

    return seq[n-1];
}
