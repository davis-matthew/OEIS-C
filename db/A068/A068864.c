/*
 * A068864  a(1) = 1; a(n+1) is the smallest square not already in the
 *          sequence which differs from a(n) at every digit.
 * https://oeis.org/A068864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 249, u32 n <= 9651, u64 n <= 9651,
 *            u128 n <= 9651, big any n
 * Helpers:   digits_differ
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS sequence A068864
 * a(1) = 1; a(n+1) is the smallest square not already in the
 * sequence which differs from a(n) at every decimal digit.  The
 * algorithm stores all previously generated terms and a bool array
 * of used square indices; each next term is found by linear search
 * over increasing integers m (checking the square m*m).  The
 * implementation is O(n²) worst‑case but fast enough for n≈10 000. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digits_differ.h"

/* ---------- global state ---------- */
static bool *used = NULL;
static size_t used_cap = 0;      /* number of bools allocated */

static seq_t *seq = NULL;
static size_t seq_len = 0;       /* number of elements stored */
static size_t seq_cap = 0;       /* allocation capacity */

/* Ensure the used[] array can contain indices up to 'need' (inclusive). */
static void ensure_used_cap(size_t need)
{
    if (need + 1 <= used_cap)
        return;

    size_t new_cap = (used_cap == 0) ? 16 : used_cap;
    while (need + 1 > new_cap)
        new_cap <<= 1;

    bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!tmp) {
        /* Allocation failed; subsequent calls cannot recover. */
        abort();
    }
    /* Initialise the new entries to false. */
    for (size_t i = used_cap; i < new_cap; ++i)
        tmp[i] = false;
    used = tmp;
    used_cap = new_cap;
}

/* Mark the square index 'idx' as used or unused. */
static void set_used_square(idx_t idx, bool val)
{
    if ((size_t)idx + 1 > used_cap)
        ensure_used_cap((size_t)idx);
    used[(size_t)idx] = val;
}

/* Test if square index 'idx' is already used. */
static bool used_square(idx_t idx)
{
    if ((size_t)idx + 1 > used_cap)
        return false;
    return used[(size_t)idx];
}

/* Ensure the seq array is allocated to hold at least 'need' elements. */
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap)
        return;

    size_t new_cap = (seq_cap == 0) ? 64 : seq_cap * 2;
    while (new_cap < need)
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) {
        abort();
    }
    seq = tmp;
    seq_cap = new_cap;
}

/* ----------- public function ----------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n=0, as per OEIS offset */

    /* Initialise sequence and used arrays on first call. */
    if (seq_len == 0) {
        ensure_seq_cap(64);
        seq[seq_len++] = (seq_t)1;   /* a(1) = 1 */
        set_used_square((idx_t)1, true);  /* 1 = 1² is used */
    }

    /* Generate terms up to the requested index. */
    while (seq_len < (size_t)n) {
        seq_t prev = seq[seq_len - 1];

        /* Search for the smallest new square differing in all digits. */
        for (idx_t m = 1;; ++m) {
            if (used_square(m))
                continue;
            seq_t cand = (seq_t)m;
            cand *= cand;
            if (!digits_differ(prev, cand))
                continue;

            /* Accept this candidate. */
            set_used_square(m, true);
            ensure_seq_cap(seq_len + 1);
            seq[seq_len++] = cand;
            break;
        }
    }

    return seq[n - 1];
}
