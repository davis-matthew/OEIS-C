/*
 * A083664  a(1) = 1, a(n) = if n mod 10 = 0 then n else smallest number m,
 *          not occurring earlier such that m*a(n-d) is a palindrome, where d
 *          = if n mod 10 = 1 then 2 else 1.
 * https://oeis.org/A083664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1; for n > 1, if n mod 10 == 0 → a(n) = n else d = (n mod
 *            10 == 1) ? 2 : 1 a(n) = smallest unused m such that m * a(n‑d)
 *            is a palindrome
 * Range:     u8 n <= 6, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------------- */
/*  No C++ constructs are used – only standard C headers are included.   */
/*  The sequence data and the set of used values are memoised in static   */
/*  dynamic arrays so that calls to a(idx_t) may occur in any order.     */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* forward declaration required by the build system */
seq_t a(idx_t n);

/* --------------------------------------------------------------------- */
/* Helper: test whether a seq_t number is a decimal palindrome           */
static inline bool is_palindrome(seq_t x)
{
    seq_t rev = 0;
    seq_t t   = x;
    while (t != 0)
    {
        rev = rev * 10 + (t % 10);
        t   = t / 10;
    }
    return rev == x;
}

/* --------------------------------------------------------------------- */
/* Storage for the sequence and for the set of used values.  All        */
/* indices are 1‑based;  aseq[0] and used[0] are unused.                 */
static seq_t *aseq       = NULL;   /* aseq[1]..aseq[last] are valid    */
static size_t aseq_cap  = 0;
static unsigned char *used  = NULL;   /* 0 = not seen, 1 = seen          */
static size_t used_cap    = 0;

/* Highest index that has already been computed and stored in aseq.    */
static idx_t last = 0;

/* --------------------------------------------------------------------- */
/* Ensure that the aseq array can hold at least indices up to n.        */
static void ensure_seq_cap(idx_t n)
{
    if ((size_t)n >= aseq_cap)
    {
        size_t new_cap = (size_t)n + 1;          /* include index 0    */
        seq_t *tmp = (seq_t *)realloc(aseq, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        aseq     = tmp;
        aseq_cap = new_cap;
        /* no need to initialise new elements; they will be written before use */
    }
}

/* --------------------------------------------------------------------- */
/* Ensure that the used array can hold values at least up to v.         */
static void ensure_used_cap(size_t v)
{
    if (v >= used_cap)
    {
        size_t old_cap = used_cap;
        size_t new_cap = v + 1;
        unsigned char *tmp = (unsigned char *)realloc(used, new_cap * sizeof(unsigned char));
        if (!tmp) exit(EXIT_FAILURE);
        used   = tmp;
        used_cap = new_cap;
        /* initialise the newly allocated part to zero */
        memset(used + old_cap, 0, new_cap - old_cap);
    }
}

/* --------------------------------------------------------------------- */
/* The function requested by the OEIS build system.                       */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* index 0 is outside the defined range */

    ensure_seq_cap(n);

    /* Compute all missing values up to n in ascending order.           */
    for (idx_t k = last + 1; k <= n; ++k)
    {
        if (k == 1)                 /* base case  */
        {
            aseq[1] = (seq_t)1;
            ensure_used_cap(1);
            used[1] = 1;
        }
        else if ((k % 10) == 0)      /* multiples of 10 */
        {
            aseq[(size_t)k] = (seq_t)k;
            ensure_used_cap((size_t)k);
            used[(size_t)k] = 1;
        }
        else                         /* general rule   */
        {
            idx_t d = ((k % 10) == 1) ? 2 : 1;
            seq_t target = aseq[(size_t)(k - d)];
            /* search for the smallest unused m such that m * target is a palindrome */
            size_t m = 1;
            for (;; ++m)
            {
                if (m >= used_cap) ensure_used_cap(m);
                if (!used[m])
                {
                    seq_t prod = (seq_t)m * target;
                    if (is_palindrome(prod))
                    {
                        aseq[(size_t)k] = (seq_t)m;
                        used[(size_t)m] = 1;
                        break;
                    }
                }
            }
        }
    }
    last = n;
    return aseq[(size_t)n];
}
