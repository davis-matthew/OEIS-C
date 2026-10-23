/*
 * A192852  Lexicographically earliest sequence such that a(n+1+a(n)) = n -
 *          a(n).
 * https://oeis.org/A192852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Lexicographically earliest sequence such that
        a(n+1+a(n)) = n - a(n)   for n ≥ 1  (offset 1)
    The function a(idx_t n) returns the n‑th term, computing the
    sequence on demand and memoizing it so that any order of calls is
    supported.  The implementation uses O(n) time and
    O(n) memory for the requested range.  */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Memoisation storage                                                */
/* ------------------------------------------------------------------ */
static seq_t *a_seq      = NULL;  /* values */
static bool  *a_filled   = NULL;  /* true if value at position is fixed */
static idx_t  computed_upto = 0;   /* largest index that has been processed */
static size_t cap          = 0;    /* current allocated array size */

/* ------------------------------------------------------------------ */
/* Ensure that the arrays can address indices up to 'need' (1‑based). */
/* ------------------------------------------------------------------ */
static void ensure_capacity(idx_t need)
{
    if (need < cap)              /* already big enough */
        return;

    /* Choose a new size, at least 64, doubling until it fits. */
    size_t new_cap = (cap >= 64) ? cap : 64;
    while (new_cap <= (size_t)need)
        new_cap <<= 1;

    a_seq    = (seq_t *)realloc(a_seq,    new_cap * sizeof(seq_t));
    a_filled = (bool  *)realloc(a_filled, new_cap * sizeof(bool));

    /* Initialise the newly added slots as 'not yet filled'. */
    for (size_t i = cap; i < new_cap; ++i)
        a_filled[i] = false;

    cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Compute a(n) for any n ≥ 1  (out of order calls are handled).      */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0u)                    /* undefined for n=0, return 0 safely */
        return 0;

    /* Make sure the array can address n. */
    ensure_capacity(n);

    /* Process every index up to n.  The recurrence must be applied
       for *all* n, even if a(n) was set earlier. */
    for (idx_t cur = 1; cur <= n; ++cur)
    {
        /* ------------------------------------------------------------------
           1. If a(cur) is not yet fixed, find the lexicographically
              smallest non‑negative candidate that is compatible with a
              possibly already fixed a(cur+1+candidate).
           ------------------------------------------------------------------ */
        if (!a_filled[cur])
        {
            /* Candidate search: cand ranges from 0 up to cur (inclusive). */
            for (seq_t cand = 0; ; ++cand)
            {
                if (cand > (seq_t)cur)       /* impossible for valid sequence */
                    break;                  /* safety; should never happen */

                idx_t m = cur + 1 + (idx_t)cand;   /* index to be fixed */
                seq_t val = (seq_t)cur - cand;     /* value to assign */

                ensure_capacity(m);

                /* Conflict with a previously fixed value? */
                if (a_filled[m] && a_seq[m] != val)
                    continue;                /* try next candidate */

                /* No conflict → commit the choice. */
                a_seq[cur]      = cand;
                a_filled[cur]   = true;

                if (!a_filled[m])
                {
                    a_seq[m]      = val;
                    a_filled[m]   = true;
                }
                break;
            }
        }

        /* ------------------------------------------------------------------
           2. Apply the recurrence for the current index.  This guarantees
              that every n satisfies a(n+1+a(n)) = n - a(n).
           ------------------------------------------------------------------ */
        seq_t cur_val = a_seq[cur];
        idx_t m       = cur + 1 + (idx_t)cur_val;
        seq_t val     = (seq_t)cur - cur_val;

        ensure_capacity(m);
        if (!a_filled[m])
        {
            a_seq[m]      = val;
            a_filled[m]   = true;
        }
    }

    computed_upto = n;
    return a_seq[(size_t)n];
}
