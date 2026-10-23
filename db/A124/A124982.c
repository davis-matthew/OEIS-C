/*
 * A124982  Nonprime numbers with a unique partition as a sum of 2 squares x^2
 *          + y^2.
 * https://oeis.org/A124982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth nonprime number that can be expressed as a sum of
 *            two squares x^2 + y^2 in exactly one unordered way
 * Range:     u8 n <= 56, u16 n <= 5719, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The algorithm generates the sequence incrementally.  For each integer m
    starting from 0 it checks whether   (i)  m is not prime (or m 0,1),
    (ii) it is a sum of two squares, and (iii) it has a unique unordered
    representation.  Condition (iii) is equivalent to the number of
    representations r₂(m) = 4 or 8, i.e. the product ∏(e_i+1) over primes
    ≡1 (mod4) is 1 or 2.  The function maintains a static cache of the
    sequence and expands a smallest-prime‑factor table on demand.  The
    function is O(n log log n) and satisfies the time and memory limits. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_spf.h"
#include "../utils/seq_realloc.h"

/* static cache for the sequence */
static seq_t *s_cache   = NULL;
static size_t  s_cap    = 0;
static size_t  s_len    = 0;

/* static smallest‑prime‑factor table  (index 0..limit) */
static idx_t *spf          = NULL;
static idx_t  spf_limit    = 0;

/* enlarge the spf table to cover at least n */
static void ensure_spf(idx_t n)
{
    if (spf_limit <= n) {
        idx_t new_limit = spf_limit ? spf_limit * 2 : 1024;
        while (new_limit <= n) new_limit <<= 1;
        idx_t *new_spf = NULL;
        build_spf(new_limit, &new_spf);
        free(spf);
        spf       = new_spf;
        spf_limit = new_limit;
    }
}

/* test whether m (m≥0) is a nonprime number that has exactly one unordered
   representation as a sum of two squares. */
static bool good_number(idx_t m)
{
    if (m == 0 || m == 1) return true;        /* 0 and 1 are special cases */

    /* make sure we can factor m */
    ensure_spf(m);

    /* prime test via spf table */
    if (spf[m] == m) return false; /* m is prime */

    idx_t  tmp    = m;
    uint64_t prod = 1;               /* product over primes ≡1 (mod4) of (exp+1) */

    while (tmp > 1) {
        idx_t p = spf[tmp];
        uint64_t e = 0;
        while ((tmp % p) == 0) {
            tmp /= p;
            ++e;
        }
        if (p == 2) {
            continue;                /* 2 does not influence representation count */
        } else if ((p & 3ULL) == 3ULL) {  /* p ≡ 3 (mod4) */
            if (e & 1) return false;     /* odd exponent -> cannot be sum of two squares */
        } else { /* p ≡ 1 (mod4) */
            prod *= (e + 1);
            if (prod > 2) return false;    /* more than one unordered representation */
        }
    }
    return (prod == 1) || (prod == 2);
}

/* main sequence accessor */
seq_t a(idx_t n)
{
    /* offset 1: a(1)=0, a(2)=1 */
    if (n == 0) return 0;            /* undefined but harmless */

    static idx_t current_m = 0;        /* next integer to test for inclusion */

    /* generate terms until we have at least n of them */
    while (s_len < n) {
        seq_t val = (seq_t)current_m;
        bool include = false;

        if (current_m <= 1) {
            include = true;            /* 0 and 1 always included */
        } else {
            include = good_number(current_m);
        }

        if (include) {
            if (s_len >= s_cap) {
                size_t new_cap = s_cap ? s_cap * 2 : 16;
                seq_t *new_cache = seq_realloc(s_cache, s_cap, new_cap);
                s_cache = new_cache;
                s_cap   = new_cap;
            }
            s_cache[s_len++] = val;
        }
        ++current_m;
    }
    return s_cache[n - 1];
}
