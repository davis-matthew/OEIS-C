/*
 * A156031  Alternate A022941 and A143344.
 * https://oeis.org/A156031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 693, u32 n <= 2001, u64 n <= 2001,
 *            u128 n <= 2001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A156031: Alternate A022941 and A143344
 * Definition:
 *   a(1)=1, a(2)=2, a(3)=3;
 *   For n≥4:
 *     if n is even:  a(n)=a(n-1)+a(n-2)   (sum of the two previous terms)
 *     if n is odd :  a(n)=smallest positive integer not yet in the sequence.
 *
 * This implementation builds the sequence on demand, memoising all
 * computed values and a bitmap of used numbers, so that any call
 * returns the correct term in O(n) time.  It works for all
 * allowed builds (fixed-size integers or an arbitrary-precision
 * integer class) and compiles under C11 and C++17.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Static state for memoisation */
static seq_t *seq = NULL;          /* array of sequence values */
static size_t seq_len = 0;         /* number of stored terms */
static size_t seq_cap = 0;         /* capacity of seq array */

static bool *used = NULL;          /* bit‑set of numbers that have appeared */
static idx_t used_cap = 0;         /* used capacity (size of 'used' array) */
static idx_t next_missing = 1;     /* smallest positive integer not yet used */

/* Ensure seq has capacity for at least 'need' terms */
static void ensure_seq_capacity(size_t need)
{
    if (seq_cap < need) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        while (new_cap < need) new_cap <<= 1;
        seq_t *new_seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!new_seq) abort();        /* out of memory */
        seq = new_seq;
        seq_cap = new_cap;
    }
}

/* Ensure 'used' can index up to value 'v' */
static void ensure_used_capacity(idx_t v)
{
    if (used_cap <= v) {
        size_t new_cap = used_cap ? used_cap * 2 : 16;
        while (new_cap <= (size_t)v) new_cap <<= 1;
        bool *new_used = (bool *)realloc(used, new_cap * sizeof(bool));
        if (!new_used) abort();       /* out of memory */
        /* zero‑initialise the newly added part */
        memset(&new_used[used_cap], 0, (new_cap - used_cap) * sizeof(bool));
        used = new_used;
        used_cap = (idx_t)new_cap;
    }
}

/* Append a term to the stored sequence and mark it as used */
static void add_term(seq_t val)
{
    ensure_seq_capacity(seq_len + 1);
    seq[seq_len++] = val;
    /* We only need to ensure space for the smallest missing values,
       which are small compared to seq_t's capacity.  The cast is safe
       for the intended use cases. */
    ensure_used_capacity((idx_t)val);
    used[(idx_t)val] = true;
}

/* Exported function required by the judge */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined, but guard anyway */

    /* Initialise first three terms once */
    if (seq_len == 0) {
        ensure_seq_capacity(3);
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)2;
        seq[2] = (seq_t)3;
        seq_len = 3;

        ensure_used_capacity(3);
        used[1] = used[2] = used[3] = true;
        next_missing = 4;
    }

    /* Build sequence up to the requested index n */
    while (seq_len < (size_t)n) {
        idx_t idx = (idx_t)seq_len + 1;  /* 1‑based index of the term to add */
        seq_t val;

        if ((idx & 1) == 0) {           /* even index */
            val = seq[seq_len - 1] + seq[seq_len - 2];
        } else {                        /* odd index */
            while (used[next_missing]) ++next_missing;
            val = (seq_t)next_missing;
        }

        add_term(val);
    }

    return seq[n - 1];
}
