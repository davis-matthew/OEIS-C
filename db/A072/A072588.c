/*
 * A072588  Numbers having at least one prime factor with an odd and one with
 *          an even exponent.
 * https://oeis.org/A072588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A072588: numbers having at least one prime factor with an odd exponent
   and at least one prime factor with an even exponent.
   The algorithm generates integers in ascending order, keeps a table of the
   smallest prime factor (SPF) of every number seen so far, and appends only
   those integers whose prime‑factorisation contains at least one odd and one
   even exponent.  The routine is memoised so that arbitrary indices can be
   requested in any order.  It is efficient for n up to several ten thousand
   (≈2 seconds in the 64‑bit build). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Small‑prime‑factor table (indices are uint64_t values).            */
/* ------------------------------------------------------------------ */
static idx_t *spf_arr = NULL;      /* SPF array, indexed by value */
static size_t spf_size = 0;        /* current maximum index that has SPF set */

/* ------------------------------------------------------------------ */
/*  Dynamic array of previously found terms (0‑based).                  */
/* ------------------------------------------------------------------ */
static seq_t *terms = NULL;        /* array of terms */
static size_t terms_len = 0;       /* number of terms stored */
static size_t terms_cap = 0;       /* capacity of the array */

/* ------------------------------------------------------------------ */
/*  Next integer to test.  Initialise at 2 – all smaller integers are
   excluded from the sequence.                                                     */
/* ------------------------------------------------------------------ */
static idx_t candidate = 2;

/* ------------------------------------------------------------------ */
/*  Build or enlarge the SPF table up to at least `limit`.  The algorithm
   recomputes the entire table from scratch, which is fast enough for
   limits of the order of a few hundred thousand.                              */
/* ------------------------------------------------------------------ */
static void build_spf(size_t limit)
{
    if (spf_arr && limit <= spf_size)
        return;                        /* already sufficient */

    idx_t *new_arr = (idx_t *)realloc(spf_arr,
                                      (limit + 1) * sizeof(idx_t));
    if (!new_arr) abort();             /* out of memory */
    spf_arr = new_arr;
    spf_size = limit;

    /* initialise to zero */
    for (size_t i = 2; i <= limit; ++i)
        spf_arr[i] = 0;

    /* linear sieve for SPF */
    for (size_t i = 2; i <= limit; ++i) {
        if (spf_arr[i] == 0) {
            spf_arr[i] = (idx_t)i;
            if (i <= limit / i) {
                for (size_t j = i * i; j <= limit; j += i) {
                    if (spf_arr[j] == 0)
                        spf_arr[j] = (idx_t)i;
                }
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Append a new term to the global `terms` array.                       */
/* ------------------------------------------------------------------ */
static void append_term(idx_t v)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap << 1 : 4;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* out of memory */
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = (seq_t)v;
}

/* ------------------------------------------------------------------ */
/*  Test whether `val` has at least one odd exponent and one even
   exponent in its prime factorisation.                                   */
/* ------------------------------------------------------------------ */
static bool has_odd_and_even_exponents(idx_t val)
{
    bool odd = false;
    bool even = false;

    while (val > 1) {
        /* Ensure SPF information is available */
        if ((size_t)val > spf_size)
            build_spf((size_t)val * 2);

        idx_t p = spf_arr[val];
        idx_t exp = 0;
        while (val % p == 0) {
            val /= p;
            ++exp;
        }
        if (exp & 1U)
            odd = true;
        else
            even = true;
        if (odd && even)
            return true;
    }
    return odd && even;
}

/* ------------------------------------------------------------------ */
/*  External routine required by the test harness.                       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for n < 1 */

    /* Prime factorisation table must be initialised at least up to
       the first few primes. */
    if (spf_size == 0)
        build_spf(1024);

    /* Generate terms until we have at least `n` of them. */
    while (terms_len < n) {
        if ((size_t)candidate > spf_size) {
            size_t new_limit = spf_size ? spf_size * 2 : 1024;
            if (new_limit <= (size_t)candidate)
                new_limit = (size_t)candidate * 2;
            build_spf(new_limit);
        }

        if (has_odd_and_even_exponents(candidate))
            append_term(candidate);

        ++candidate;
    }

    /* a(1) is stored at terms[0] */
    return terms[n - 1];
}
