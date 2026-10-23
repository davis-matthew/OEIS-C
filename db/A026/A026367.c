/*
 * A026367  a(n) = least k such that s(k) = n, where s = A026366.
 * https://oeis.org/A026367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2; for n>2, a(n)=a(n-1)+2 if n∈{2a(k)+1 for some
 *            k≥1}, a(n)=a(n-1)+1 otherwise
 * Range:     u8 n <= 187, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Standard C library for memory management.
    The seq_t type is provided by the build system and may be,
    e.g., uint32_t, uint64_t, unsigned __int128, or an arbitrary‑precision class.
    idx_t is the unsigned 64‑bit index type (usually uint64_t).  */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ------------------  Static memoisation state  ------------------ */
static seq_t *a_values = NULL;   /* array of computed a(n), 1‑based; a_values[0] unused */
static size_t a_cap = 0;         /* capacity of a_values array (number of slots)      */
static idx_t  a_max  = 0;         /* largest n for which a_values[n] is valid           */
static seq_t  last_a = 0;         /* a(a_max) for fast incremental computation */

/* bit array: 1 if value v has appeared as some a(k); otherwise 0.
   Indexed by the value itself (the values grow slowly for n≈10 000).  */
static unsigned char *is_a_value = NULL;
static idx_t  is_a_cap = 0;        /* current capacity (number of bytes) of is_a_value */

/* Ensure a_values has at least 'need' places (need ≥ 1).  */
static void ensure_a_cap(size_t need)
{
    if (a_cap >= need) return;
    size_t new_cap = a_cap ? a_cap : 4;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_values, new_cap * sizeof(seq_t));
    if (!tmp) abort();                            /* out‑of‑memory */
    a_values = tmp;
    a_cap = new_cap;
}

/* Ensure is_a_value can be indexed by 'need' (need ≥ 0).  */
static void ensure_is_a_cap(idx_t need)
{
    if (is_a_cap > need) return;                  /* capacity already exceeds need */
    idx_t new_cap = is_a_cap ? is_a_cap : 4;
    while (new_cap <= need) new_cap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(is_a_value, new_cap);
    if (!tmp) abort();
    /* Zero the new memory that becomes part of the array. */
    memset(tmp + is_a_cap, 0, (size_t)(new_cap - is_a_cap));
    is_a_value = tmp;
    is_a_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Primary sequence routine.                                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Offset 1; a(0) is undefined. */
    if (n == 0) return 0;

    /* Initialise static storage on the first call. */
    if (!a_values) {
        a_values = (seq_t *)malloc(4 * sizeof(seq_t));
        if (!a_values) abort();
        memset(a_values, 0, 4 * sizeof(seq_t));
        a_cap = 4;

        is_a_value = (unsigned char *)malloc(4);
        if (!is_a_value) abort();
        memset(is_a_value, 0, 4);
        is_a_cap = 4;

        a_values[1] = 1;          /* a(1) = 1 */
        last_a = 1;
        a_max = 1;
        is_a_value[1] = 1;        /* mark 1 as an a‑value */
    }

    /* If the requested n is already known, return it immediately. */
    if (n <= a_max) return a_values[n];

    /* Compute a(n) iteratively up to the requested index. */
    for (idx_t i = a_max + 1; i <= n; ++i) {
        bool inSet = false;

        /* n is in 2A+1 only if n is odd and (n‑1)/2 has appeared as an a‑value. */
        if ((i & 1U) != 0) {                /* odd i */
            seq_t m = ((seq_t)i - 1) / 2;   /* (i-1)/2 */
            idx_t idx = (idx_t)m;
            if (idx < is_a_cap && is_a_value[(size_t)idx]) {
                inSet = true;
            }
        }

        /* Update last_a according to the recurrence. */
        last_a += inSet ? 2 : 1;

        /* Store the new a(i). */
        ensure_a_cap((size_t)i + 1);        /* guard against overflow of a_cap */
        a_values[i] = last_a;

        /* Mark this value as having been seen. */
        idx_t val = (idx_t)last_a;
        ensure_is_a_cap(val);
        is_a_value[(size_t)val] = 1;

        a_max = i;                         /* update highest computed index */
    }

    return a_values[n];
}
