/*
 * A213183  Initialize a(1)=R=1. Repeat: copy the last R preceding terms to
 *          current position; increment R; do twice: append the least integer
 *          that has not appeared in the sequence yet.
 * https://oeis.org/A213183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates OEIS A213183.
   Rules: a(1)=1, R=1.  Then repeat:
     copy the last R terms,
     R ← R+1,
     append twice the smallest positive integer not yet in the sequence. */

#include <stdlib.h>
#include <string.h>

/* static state */
static seq_t *seq = NULL;          /* sequence buffer */
static size_t seq_cap = 0;         /* capacity of seq */
static size_t seq_len = 0;         /* current length of seq */

static unsigned char *seen = NULL; /* seen flags for small integer values */
static size_t seen_cap = 0;        /* capacity of seen */

/* Ensure seen[0..needed] exists (inclusive). */
static void ensure_seen_capacity(size_t needed)
{
    if (needed < seen_cap) return;
    size_t new_cap = seen_cap ? seen_cap : 2;
    while (new_cap <= needed) new_cap <<= 1;
    unsigned char *p = (unsigned char *)realloc(seen, new_cap);
    if (!p) abort();
    /* zero the newly allocated part */
    memset(p + seen_cap, 0, new_cap - seen_cap);
    seen = p;
    seen_cap = new_cap;
}

/* Mark that the integer value v has appeared. */
static void mark_seen(seq_t v)
{
    size_t idx = (size_t)v;           /* values fit in size_t */
    ensure_seen_capacity(idx);
    seen[idx] = 1;
}

/* Find the smallest positive integer not yet seen. 1 is already present. */
static seq_t find_least_missing(void)
{
    static seq_t min_missing = 2;      /* start at 2 */
    while (1) {
        size_t idx = (size_t)min_missing;
        if (idx >= seen_cap || !seen[idx]) return min_missing;
        ++min_missing;
    }
}

/* Grow seq to capacity at least 'need' elements. */
static void grow_seq(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_t *p = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!p) abort();
    seq = p;
    seq_cap = new_cap;
}

/* Main function requested. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined, safe fallback */

    if (!seq) {                       /* first call: initialize */
        seq_cap = 64;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) abort();
        seq[0] = (seq_t)1;
        seq_len = 1;

        seen_cap = 2;                 /* need indices 0 and 1 */
        seen = (unsigned char *)calloc(seen_cap, 1);
        if (!seen) abort();
        seen[1] = 1;                  /* 1 has appeared */
    }

    static size_t R = 1;              /* number of terms to copy next */

    /* Build sequence until it contains at least n terms. */
    while (seq_len < (size_t)n) {
        /* Copy the last R terms. */
        grow_seq(seq_len + R);
        for (size_t i = 0; i < R; ++i) {
            seq[seq_len + i] = seq[seq_len - R + i];
        }
        seq_len += R;

        /* Prepare for next iteration. */
        ++R;

        /* Append twice the smallest missing integer. */
        grow_seq(seq_len + 2);
        for (int k = 0; k < 2; ++k) {
            seq_t val = find_least_missing();
            seq[seq_len + k] = val;
            mark_seen(val);
        }
        seq_len += 2;
    }

    return seq[n - 1];
}
