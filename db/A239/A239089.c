/*
 * A239089  The sequence S = a(1), a(2), ... is defined by a(1)=1, if d,e,f
 *          are consecutive digits then we do not have d > e >= f, and S is
 *          always extended with the smallest integer not yet present in S.
 * https://oeis.org/A239089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest integer not yet present that, when appended to
 *            the decimal concatenation of the sequence so far, avoids any
 *            triple (d, e, f) of consecutive digits with d > e >= f
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>

/* State of the sequence */
static seq_t *seq = NULL;          /* stored terms */
static size_t seq_len = 0;         /* number of stored terms  */
static size_t seq_cap = 0;         /* array capacity of seq */

static uint8_t *used = NULL;      /* bitmap of used integers */
static size_t used_cap = 0;        /* capacity of used */

/* Last two digits of the concatenated sequence (-1 means undefined) */
static int last_digit_1 = -1;      /* second‑last digit of concatenation */
static int last_digit_2 = -1;      /* last digit of concatenation */

/* Ensure that used[idx] is allocated */
static void ensure_used_capacity(size_t idx)
{
    if (idx < used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 64;
    while (new_cap <= idx) new_cap *= 2;
    uint8_t *tmp = (uint8_t *)realloc(used, new_cap);
    if (!tmp) exit(1);
    memset(tmp + used_cap, 0, new_cap - used_cap);
    used = tmp;
    used_cap = new_cap;
}

/* Ensure that seq array has at least need capacity */
static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    seq = tmp;
    seq_cap = new_cap;
}

/* Test whether appending cand preserves the forbidden‑triple rule.
   If valid, new_last1 and new_last2 are set to the last two digits
   of the concatenation after appending cand. */
static bool
can_append(seq_t cand, int *new_last1, int *new_last2)
{
    int prev1 = last_digit_1;
    int prev2 = last_digit_2;

    /* Extract digits of cand into a buffer (reversed order). */
    unsigned char rev[64];
    size_t len = 0;
    seq_t v = cand;
    if (v == (seq_t)0) {
        rev[len++] = 0;
    } else {
        while (v > (seq_t)0) {
            rev[len++] = (unsigned char)(v % (seq_t)10);
            v /= (seq_t)10;
        }
    }

    /* Process digits from most significant to least significant. */
    for (size_t i = 0; i < len; ++i) {
        unsigned char d = rev[len - 1 - i];
        if (prev1 != -1 && prev2 != -1) {
            if (prev1 > prev2 && prev2 >= d) {
                return false;
            }
        }
        prev1 = prev2;
        prev2 = d;
    }
    *new_last1 = prev1;
    *new_last2 = prev2;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* safety, sequence is 1‑based */

    /* Allocate storage for the sequence if needed. */
    if (!seq) {
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) return (seq_t)0;
        used = (uint8_t *)calloc(used_cap, sizeof(uint8_t));
        if (!used) return (seq_t)0;
    }

    /* Generate terms until we have at least n terms. */
    while (seq_len < (size_t)n) {
        /* Find the smallest integer not yet present that is valid. */
        size_t cand = 1;
        for (;; ++cand) {
            ensure_used_capacity(cand);
            if (used[cand]) continue;    /* already in the sequence */

            int new1, new2;
            if (can_append((seq_t)cand, &new1, &new2)) {
                /* Append the term. */
                ensure_seq_capacity(seq_len + 1);
                seq[seq_len++] = (seq_t)cand;
                used[cand] = 1;
                last_digit_1 = new1;
                last_digit_2 = new2;
                break;                    /* go to next term */
            }
            /* otherwise keep searching for a valid candidate */
        }
    }

    return seq[n - 1];
}
