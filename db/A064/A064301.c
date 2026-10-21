/*
 * A064301  Rightmost column of A064744; 2-adic valuation of the EKG-sequence.
 * https://oeis.org/A064301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = exponent of the highest power of 2 dividing the n-th term
 *            of the EKG sequence
 * Range:     u8 n <= 105, u16 n <= 9137, u32 n <= 9137, u64 n <= 9137,
 *            u128 n <= 9137, big any n
 * Helpers:   gcd, trailing_zeros
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The EKG sequence: a(1)=1; for n>1, if a(n‑1)=1 then a(n)=2; otherwise a(n) is the smallest unused integer that shares a factor >1 with a(n‑1). */
/* This implementation lazily builds the EKG sequence and memoises the 2‑adic valuation. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/trailing_zeros.h"

static seq_t *terms = NULL;           /* actual EKG terms */
static seq_t *vals  = NULL;           /* 2‑adic valuations of the terms */
static size_t  terms_cap = 0;
static size_t  vals_cap  = 0;
static idx_t   max_computed = 0;      /* largest n computed so far */

static bool *used = NULL;             /* bitmap of numbers already used */
static idx_t  used_cap = 0;

/* Grow the used array to at least min_cap entries. */
static void ensure_used_capacity(idx_t min_cap)
{
    if (used_cap >= min_cap) return;
    size_t new_cap = used_cap ? used_cap << 1 : 1;
    while (new_cap < (size_t)min_cap) new_cap <<= 1;
    bool *new_used = (bool*)realloc(used, new_cap * sizeof(bool));
    if (!new_used) exit(1);
    for (size_t i = used_cap; i < new_cap; ++i) new_used[i] = false;
    used = new_used;
    used_cap = new_cap;
}

/* Grow the terms/vals arrays to at least min_cap entries. */
static void ensure_terms_capacity(idx_t min_cap)
{
    if (terms_cap >= min_cap) return;
    size_t new_cap = terms_cap ? terms_cap << 1 : 1;
    while (new_cap < (size_t)min_cap) new_cap <<= 1;
    seq_t *new_terms = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
    seq_t *new_vals  = (seq_t*)realloc(vals,  new_cap * sizeof(seq_t));
    if (!new_terms || !new_vals) exit(1);
    terms = new_terms;
    vals  = new_vals;
    terms_cap = vals_cap = new_cap;
}

/* Compute the sequence up to the given n (1‑based). */
static void compute_up_to(idx_t n)
{
    if (n <= max_computed) return;

    ensure_terms_capacity(n);
    ensure_used_capacity(4);          /* need indices up to 3 for the first term */

    if (max_computed == 0)            /* initialise first term */
    {
        used[1] = true;
        terms[0] = (seq_t)1;
        vals[0]  = trailing_zeros((seq_t)1);
        max_computed = 1;
    }

    for (idx_t idx = max_computed; idx < n; ++idx)
    {
        seq_t prev = terms[idx - 1];
        seq_t term;
        if (prev == (seq_t)1)         /* special rule after 1 */
        {
            term = (seq_t)2;
            ensure_used_capacity(3);
            used[2] = true;
        }
        else
        {
            idx_t c = 2;
            for (;; ++c)
            {
                ensure_used_capacity(c + 1);
                if (!used[c])
                {
                    seq_t g = gcd(prev, (seq_t)c);
                    if (g != (seq_t)1)
                    {
                        used[c] = true;
                        term = (seq_t)c;
                        break;
                    }
                }
            }
        }
        terms[idx] = term;
        vals[idx]  = trailing_zeros(term);
    }
    max_computed = n;
}

/* Public function required by the system. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, harmless */
    compute_up_to(n);
    return vals[n - 1];
}
