/*
 * A202024  Lexicographically earliest positive integer sequence such that no
 *          sum of any number of consecutive terms is an integer of the form
 *          k^2+k+1 for any positive integer k.
 * https://oeis.org/A202024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Lexicographically earliest positive integer sequence such that
 *            no sum of any number of consecutive terms equals k^2+k+1 for any
 *            positive integer k. Recurrence: a(n) is the smallest positive
 *            integer that, when appended to the prefix a(1)…a(n-1), does not
 *            create a forbidden sum as defined above
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   prefix_has_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/prefix_has_value.h"

/* Dynamic storage for the sequence and its prefix sums. */
static seq_t *seq      = NULL;   /* sequence values, indexed 0..len-1 */
static size_t seq_cap  = 0;

static seq_t *prefix   = NULL;   /* prefix[i] = sum of first i terms, i=0..len */
static size_t pref_cap = 0;

/* Ensure seq can hold at least 'need' elements. */
static void ensure_seq_cap(size_t need) {
    if (need <= seq_cap) return;
    size_t nc = seq_cap ? seq_cap << 1 : 8;
    while (nc < need) nc <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, nc * sizeof(seq_t));
    seq = tmp;
    seq_cap = nc;
}

/* Ensure prefix can hold at least 'need' elements. */
static void ensure_pref_cap(size_t need) {
    if (need <= pref_cap) return;
    size_t nc = pref_cap ? pref_cap << 1 : 8;
    while (nc < need) nc <<= 1;
    seq_t *tmp = (seq_t *)realloc(prefix, nc * sizeof(seq_t));
    prefix = tmp;
    pref_cap = nc;
}

/* Check whether candidate 'cand' can be appended to the sequence without
   creating a forbidden sum. */
static bool candidate_ok(seq_t cand, seq_t prefix_cur, size_t len,
                         const seq_t *pref) {
    seq_t new_prefix = prefix_cur + cand;
    idx_t new_pref_i = (idx_t)new_prefix;
    for (idx_t k = 1; ; ++k) {
        idx_t forb = k * k + k + 1;          /* forbidden sum */
        if (forb > new_pref_i) break;
        seq_t need = (seq_t)(new_pref_i - forb);  /* required previous sum */
        if (prefix_has_value(len, need, pref))
            return false;                      /* forbidden sum would occur */
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* undefined for n<1, safe for caller */

    /* ---------- Initialization ---------- */
    if (!seq) {
        seq_cap  = 8;
        seq = (seq_t *)calloc(seq_cap, sizeof(seq_t));

        pref_cap = 9;                  /* need space for prefix[0..1] at start */
        prefix = (seq_t *)calloc(pref_cap, sizeof(seq_t));
        prefix[0] = (seq_t)0;           /* sum of zero terms */
    }

    /* ---------- Grow sequence up to length n ---------- */
    size_t len = 0;                     /* number of terms already stored */
    while (len < (size_t)n) {
        seq_t prefix_cur = prefix[len]; /* sum of first len terms */

        seq_t cand = 1;                 /* smallest positive integer to try */
        while (true) {
            if (candidate_ok(cand, prefix_cur, len, prefix)) {
                /* Append candidate */
                ensure_seq_cap(len + 1);
                seq[len] = cand;
                ++len;

                /* Update prefix array */
                ensure_pref_cap(len + 1);                   /* need space for new_prefix */
                prefix[len] = prefix_cur + cand;
                break;                                     /* next position */
            }
            ++cand;
        }
    }

    /* ---------- Return the requested term ---------- */
    return seq[(size_t)n - 1];
}
