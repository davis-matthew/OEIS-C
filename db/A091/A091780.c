/*
 * A091780  Alternately odd and even numbers such that a(n) is the least
 *          number not included earlier and has a common factor with a(n-1).
 * https://oeis.org/A091780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 3. For n > 1, a(n) is the least integer not yet used that
 *            (1) has opposite parity to a(n-1) and (2) shares a non‑trivial
 *            common factor with a(n-1)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: this implementation builds the sequence on demand
   and memoises previously generated terms. */

#include <stdlib.h>

/* Euclidean algorithm for seq_t (works for all built‑in and
   arbitrary‑precision unsigned types). */
static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != 0)
    {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *seq = NULL;       /* generated terms */
    static seq_t *used = NULL;     /* bit mask of used numbers */
    static size_t seq_cap  = 0;
    static size_t used_cap = 0;
    static idx_t seq_len = 0;       /* number of terms stored */

    /* initialise on first call */
    if (!seq)
    {
        seq_cap = 256;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }
    if (!used)
    {
        used_cap = 256;
        used = (seq_t *)calloc(used_cap, sizeof(seq_t));
    }

    /* extend the sequence until we have at least n terms */
    while (seq_len < n)
    {
        seq_t prev;          /* previous term */
        seq_t cand;          /* candidate for the next term   */

        /* first term is known */
        if (seq_len == 0)
        {
            cand = 3;
            prev = 0;   /* dummy */
        }
        else
        {
            prev = seq[seq_len - 1];
            /* search for the next term */
            cand = 2;
            for (;; ++cand)
            {
                size_t idx = (size_t)cand;
                /* grow used array if necessary */
                if (idx >= used_cap)
                {
                    size_t new_cap = used_cap * 2;
                    if (new_cap < idx + 1)
                        new_cap = idx + 1;
                    used = (seq_t *)realloc(used, new_cap * sizeof(seq_t));
                    /* new memory is uninitialised: zero it */
                    for (size_t i = used_cap; i < new_cap; ++i)
                        used[i] = 0;
                    used_cap = new_cap;
                }

                /* candidate must be unused, opposite parity, and share a gcd>1 */
                if (used[idx]) continue;
                if ( (cand & 1) == (prev & 1) ) continue;   /* same parity */
                if (seq_gcd(prev, cand) == 1) continue;     /* no common factor */
                break; /* found suitable cand */
            }
        }

        /* store the new term */
        if (seq_len >= seq_cap)
        {
            seq_cap *= 2;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        }
        seq[seq_len++] = cand;

        /* mark this number as used */
        size_t idx = (size_t)cand;
        if (idx >= used_cap)
        {
            size_t new_cap = used_cap * 2;
            if (new_cap < idx + 1)
                new_cap = idx + 1;
            used = (seq_t *)realloc(used, new_cap * sizeof(seq_t));
            for (size_t i = used_cap; i < new_cap; ++i)
                used[i] = 0;
            used_cap = new_cap;
        }
        used[idx] = 1;
    }

    /* return the requested term (1‑based indexing) */
    return seq[n - 1];
}
