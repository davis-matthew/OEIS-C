/*
 * A048991  Write down the numbers 1,2,3,... but omit any number (such as 12
 *          or 23 or 31 ...) which appears in the concatenation of all earlier
 *          terms.
 * https://oeis.org/A048991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 184, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements OEIS A048991:
   a(1)=1, a(2)=2, …; a(n) is the n‑th integer that does NOT
   occur as a contiguous decimal substring of the concatenation
   of all earlier accepted terms.  The code constructs the terms
   in increasing order, stores them in a cache and returns the
   requested term.  No hard‑coded values are used.  */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

/* --- static state for memoisation ------------------------------------ */
static seq_t *seq_cache   = NULL;   /* array of computed terms   */
static size_t seq_cap     = 0;      /* capacity of seq_cache     */

static char *concat_buf   = NULL;   /* concatenated accepted terms */
static size_t concat_len  = 0;      /* current length (no NUL)   */
static size_t concat_cap  = 0;      /* capacity including NUL    */

static idx_t next_candidate = 1;    /* next integer to test      */
static idx_t generated      = 0;    /* number of terms already generated */

/* -------------------------------------------------------------------- */
static void ensure_seq_cache(idx_t need)
{
    if (seq_cap >= (size_t)need)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < (size_t)need)
        new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap   = new_cap;
}

static void ensure_concat_buf(size_t need)
{
    if (concat_cap >= need)
        return;
    size_t new_cap = concat_cap ? concat_cap * 2 : 64;
    while (new_cap < need)
        new_cap <<= 1;
    concat_buf = (char *)realloc(concat_buf, new_cap);
    concat_cap = new_cap;
}

static void append_to_concatenation(const char *s, size_t len)
{
    ensure_concat_buf(concat_len + len + 1); /* +1 for NUL */
    memcpy(concat_buf + concat_len, s, len);
    concat_len += len;
    concat_buf[concat_len] = '\0';
}

/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialise buffers on first call */
    if (!concat_buf) {
        concat_buf = (char *)malloc(1);
        concat_buf[0] = '\0';
        concat_cap = 1;
        concat_len = 0;
    }

    /* return cached value if already computed */
    if (n <= generated)
        return seq_cache[n - 1];

    /* generate terms until we have at least n terms */
    char num_buf[32];
    while (generated < n) {
        /* convert candidate to decimal string */
        int num_len = snprintf(num_buf, sizeof num_buf, "%llu",
                               (unsigned long long)next_candidate);
        /* num_len is number of chars written, excluding NUL */

        /* check whether this candidate appears in earlier terms */
        if (strstr(concat_buf, num_buf) == NULL) {
            /* accept the candidate */
            ensure_seq_cache(generated + 1);
            seq_cache[generated] = (seq_t)next_candidate;
            append_to_concatenation(num_buf, (size_t)num_len);
            ++generated;
        }
        ++next_candidate;
    }

    return seq_cache[n - 1];
}
