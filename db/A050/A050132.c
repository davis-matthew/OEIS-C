/*
 * A050132  a(n) = floor(a(n-1)/2) if this is not among 0,a(1),...,a(n-1);
 *          otherwise a(n) = 3*n.
 * https://oeis.org/A050132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A050132
   a(1) = 1
   a(n) = floor(a(n-1)/2)   if that value has never occurred before
   a(n) = 3*n               otherwise
   The sequence is a permutation of the natural numbers.
*/
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

static seq_t *seq   = NULL;   /* sequence values, 1‑based index */
static size_t seq_cap   = 0;  /* allocated capacity of seq array   */
static size_t seq_last  = 0;  /* largest index currently computed  */

static bool *seen   = NULL;   /* bitmap of values that have appeared */
static size_t seen_cap = 0;   /* allocated capacity of seen array     */

/* -------------------------------------------------------------------------- */
/* Allocate or grow the seq array so that it can hold at least 'need' entries. */
static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap)
        return;

    size_t new_cap = need * 2;
    if (new_cap < need)          /* overflow check */
        new_cap = (size_t)-1;

    seq_t *new_seq = NULL;
#ifdef __cplusplus
    new_seq = new seq_t[new_cap];
    if (seq) {
        /* copy old values */
        for (size_t i = 0; i < seq_cap; ++i)
            new_seq[i] = seq[i];
        delete[] seq;
    }
#else
    new_seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
#endif
    seq = new_seq;
    seq_cap = new_cap;
}

/* -------------------------------------------------------------------------- */
/* Allocate or grow the seen array so that it can hold indices up to 'need-1'. */
static void ensure_seen_capacity(size_t need)
{
    if (need <= seen_cap)
        return;

    size_t old_cap = seen_cap;
    size_t new_cap = need * 2;
    if (new_cap < need)          /* overflow check */
        new_cap = (size_t)-1;

    bool *tmp = NULL;
#ifdef __cplusplus
    tmp = new bool[new_cap];
    if (seen) {
        for (size_t i = 0; i < old_cap; ++i)
            tmp[i] = seen[i];
    }
    delete[] seen;
#else
    tmp = (bool *)realloc(seen, new_cap * sizeof(bool));
#endif
    /* initialise new slots to false */
    if (tmp && new_cap > old_cap)
        memset(tmp + old_cap, 0, (new_cap - old_cap) * sizeof(bool));
    seen = tmp;
    seen_cap = new_cap;
}

/* -------------------------------------------------------------------------- */
/* Initialise the static state on first call. */
static void initialise(void)
{
    if (seq)           /* already initialised */
        return;

    ensure_seq_capacity(2);          /* indices 0 and 1 */
    seq[1] = (seq_t)1;               /* a(1) = 1 */
    seq_last = 1;

    ensure_seen_capacity(2);          /* indices 0 and 1 */
    seen[0] = true;                   /* 0 is considered used */
    seen[1] = true;                   /* a(1) = 1 */
}

/* -------------------------------------------------------------------------- */
/* Main sequence generator.  Works for any n >= 1 in any call order. */
seq_t a(idx_t n)
{
    initialise();

    /* Extend the sequence up to n if necessary. */
    if ((size_t)n > seq_last) {
        size_t new_last = (size_t)n;
        ensure_seq_capacity(new_last + 1);           /* +1 because seq is 1‑based */
        ensure_seen_capacity((size_t)(3 * new_last) + 2); /* 3*n may be max value */

        for (size_t i = seq_last + 1; i <= new_last; ++i) {
            seq_t prev = seq[i - 1];
            seq_t cand = prev / (seq_t)2;      /* floor(a(n-1)/2) */

            /* Make sure the seen array is large enough to hold cand */
            size_t cand_idx = (size_t)(idx_t)cand;
            if (cand_idx >= seen_cap)
                ensure_seen_capacity(cand_idx + 1);

            seq_t val;
            if (!seen[cand_idx])
                val = cand;
            else
                val = (seq_t)((idx_t)i * (idx_t)3);

            seq[i] = val;

            /* Mark the chosen value as seen, extending the bitmap if needed. */
            size_t val_idx = (size_t)(idx_t)val;
            if (val_idx >= seen_cap)
                ensure_seen_capacity(val_idx + 1);
            seen[val_idx] = true;
        }
        seq_last = new_last;
    }

    return seq[(size_t)n];
}
