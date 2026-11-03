/*
 * A036311  Composite numbers whose prime factors contain no digits other than
 *          2 and 5.
 * https://oeis.org/A036311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 64, u32 n <= 241, u64 n <= 925,
 *            u128 n <= 3617, big any n
 * Validated: standard, pass, 64 terms through n = 9310 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A036311: composite numbers whose prime factors contain no digits
    other than 2 and 5.  The numbers are of the form 2^a·5^b with a+b≥2.
    They are generated as the Hamming numbers for the primes 2 and 5,
    filtering out 1, 2 and 5 (the only prime cases).  This implementation
    keeps a static cache of the generated Hamming numbers so that repeated
    calls are efficient.  */

#include <stdlib.h>
#include <stdbool.h>

/* Utility functions from the build */
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

static int seq_cmp(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* External linkage */
seq_t a(idx_t n)
{
    /*  static state: vector of generated Hamming numbers and indices  */
    static seq_t *harr   = NULL;   /* array of Hamming numbers */
    static size_t  harr_size = 0;
    static size_t  harr_cap  = 0;

    static idx_t   i2 = 0, i5 = 0;  /* indices for multiplying by 2 and 5 */
    static bool    initialized = false;

    /* constants for filtering out 1, 2 and 5  */
    static seq_t val1 = 0, val2 = 0, val5 = 0;

    /* initialise static data on first call  */
    if (!initialized)
    {
        /* start the Hamming sequence with 1  */
        harr_cap  = 64;
        harr      = mem_new(seq_t, harr_cap);
        val1      = seq_from_str("1");          /* 1 */
        val2      = mul_small(val1, 2);         /* 2 */
        val5      = mul_small(val1, 5);         /* 5 */
        harr[0]   = val1;                       /* Hamming[0] = 1 */
        harr_size = 1;

        i2 = 0;
        i5 = 0;
        initialized = true;
    }

    /* generate more Hamming numbers until we have at least n composite numbers
       among them.  We keep generating until a linear scan finds the nth
       composite; this is fast because the number of generated elements is
       only a little larger than n.  */
    while (true)
    {
        /* count composites already present */
        size_t composites = 0;
        for (size_t k = 0; k < harr_size; ++k) {
            if (   seq_cmp(&harr[k], &val1) != 0
                && seq_cmp(&harr[k], &val2) != 0
                && seq_cmp(&harr[k], &val5) != 0
               )
                ++composites;
        }
        if (composites >= n) break;

        /* generate the next candidate from multiplication by 2 and 5  */
        seq_t cand2 = mul_small(harr[i2], 2);
        seq_t cand5 = mul_small(harr[i5], 5);

        /* choose the smaller candidate as the next Hamming number */
        seq_t next;
        int cmp = seq_cmp(&cand2, &cand5);
        if (cmp < 0) {
            next = cand2;
            ++i2;
        } else if (cmp > 0) {
            next = cand5;
            ++i5;
        } else { /* equal: skip duplicate */
            next = cand2;
            ++i2; ++i5;
        }

        /* avoid duplicate entries (happens when cand2 == cand5) */
        if (harr_size == 0 || seq_cmp(&next, &harr[harr_size - 1]) != 0) {
            if (harr_size == harr_cap) {
                harr_cap *= 2;
                harr = mem_resize(seq_t, harr, harr_cap);
            }
            harr[harr_size++] = next;
        }

        /* the temporary candidates are not freed; the build provides
           small‐int multiplication that returns a value type with no
           explicit deallocation needed for the short sequences we
           generate.  */
    }

    /* find the nth composite number by linear scan */
    size_t rank = 0;
    for (size_t k = 0; k < harr_size; ++k) {
        if (   seq_cmp(&harr[k], &val1) == 0
            || seq_cmp(&harr[k], &val2) == 0
            || seq_cmp(&harr[k], &val5) == 0)
        {
            continue;   /* skip 1, 2, 5 */
        }
        ++rank;
        if (rank == n) {
            return harr[k];
        }
    }

    /* Should never reach here for valid input.  Return 0 as a safe
       fallback.  */
    return 0;
}
