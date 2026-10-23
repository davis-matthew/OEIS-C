/*
 * A127116  n! in base 9.
 * https://oeis.org/A127116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the representation of n! in base 9, written in decimal *
 *            (i.e. convert n! to base 9, then read the base‑9 digits as a
 *            decimal * integer). The values are cached so subsequent calls
 *            are O(1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   fact_to_base9_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/fact_to_base9_seq.h"

/* The build supplies seq_t and idx_t.  seq_t may be an arbitrary‑precision
 * integer class when compiled as C++.  All operations used below are legal
 * on both fixed‑size integers and such a class (mod, division,
 * multiplication, addition, etc.). */

/* ---------------------------------------------------------------------- */
/*  Factorial cache                                                       */
/* ---------------------------------------------------------------------- */
static seq_t *fact_cache = NULL;   /* fact_cache[i] == i!  */
static size_t fact_cap   = 0;
static idx_t fact_size   = 0;

/* ---------------------------------------------------------------------- */
/*  Sequence cache                                                       */
/* ---------------------------------------------------------------------- */
static seq_t *seq_cache = NULL;    /* seq_cache[i] == a(i) */
static size_t seq_cap   = 0;
static idx_t seq_size   = 0;

/* ---------------------------------------------------------------------- */
/*  Ensure factorials up to n                                            */
/* ---------------------------------------------------------------------- */
static void ensure_factorials(idx_t n)
{
    if (fact_cache == NULL) {
        fact_cap   = 4;
        fact_cache = (seq_t *)realloc(NULL, fact_cap * sizeof(seq_t));
        if (!fact_cache) exit(1);
        fact_cache[0] = (seq_t)1;     /* 0! = 1 */
        fact_size    = 1;
    }
    if (n < fact_size) return;

    if (n >= fact_cap) {
        while (fact_cap <= (size_t)n) fact_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(fact_cache, fact_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        fact_cache = tmp;
    }

    for (idx_t i = fact_size; i <= n; ++i) {
        fact_cache[i] = fact_cache[i-1] * (seq_t)i;
    }
    fact_size = n + 1;
}

/* ---------------------------------------------------------------------- */
/*  Convert a factorial (given by reference) to its base‑9 decimal form   */

/* ---------------------------------------------------------------------- */
/*  Ensure sequence values up to n                                        */
/* ---------------------------------------------------------------------- */
static void ensure_sequence(idx_t n)
{
    if (seq_cache == NULL) {
        seq_cap   = 4;
        seq_cache = (seq_t *)realloc(NULL, seq_cap * sizeof(seq_t));
        if (!seq_cache) exit(1);
        seq_size  = 0;
    }
    if (n < seq_size) return;

    if (n >= seq_cap) {
        while (seq_cap <= (size_t)n) seq_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        seq_cache = tmp;
    }

    /* Compute and cache all missing a(k).  The factorial values are
       guaranteed to exist because ensure_factorials has been called by
       the caller. */
    for (idx_t k = seq_size; k <= n; ++k) {
        seq_cache[k] = fact_to_base9_seq(&fact_cache[k]);
    }
    seq_size = n + 1;
}

/* ---------------------------------------------------------------------- */
/*  Public sequence function                                              */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Compute factorials and sequence values up to n. */
    ensure_factorials(n);
    ensure_sequence(n);
    return seq_cache[n];
}
