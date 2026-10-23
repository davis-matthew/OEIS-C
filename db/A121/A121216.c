/*
 * A121216  a(1)=1, a(2) = 2; thereafter a(n) = the smallest positive integer
 *          which does not occur earlier in the sequence and which is coprime
 *          to a(n-2).
 * https://oeis.org/A121216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 252, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A121216:
   a(1) = 1, a(2) = 2;
   for n > 2, a(n) = the smallest positive integer which has not
   appeared earlier and is coprime to a(n-2). */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* static storage for the sequence: seq[0] corresponds to a(1) */
static size_t seq_len    = 0; /* number of elements stored */
static seq_t *seq        = NULL;
static size_t seq_cap    = 0;

/* static storage for used numbers (index == number) */
static bool *used        = NULL;
static size_t used_cap   = 0;    /* highest number currently covered */

/* ensure that the used array covers indices up to min_index (inclusive) */
static void
ensure_used_capacity(idx_t min_index)
{
    if ((size_t)min_index <= used_cap)
        return;

    size_t new_cap = used_cap ? used_cap * 2 : 4;
    while (new_cap < (size_t)min_index)
        new_cap <<= 1;

    bool *tmp = (bool*)realloc(used, (new_cap + 1) * sizeof(bool));
    for (size_t i = used_cap + 1; i <= new_cap; ++i)
        tmp[i] = false;
    used = tmp;
    used_cap = new_cap;
}

/* produce a(n) for any n >= 1, in any order of calls */
seq_t a(idx_t n)
{
    if (n == 0)  /* undef. return 0 for safety */
        return 0;

    /* lazy initialization */
    if (seq_len == 0) {
        seq_cap = 4;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)1;   /* a(1) */
        seq[1] = (seq_t)2;   /* a(2) */
        seq_len = 2;

        used_cap = 4;
        used = (bool*)calloc(used_cap + 1, sizeof(bool));
        used[1] = true;
        used[2] = true;
    }

    /* generate terms up to the requested index */
    while (seq_len < (size_t)n) {
        seq_t prevprev = seq[seq_len - 2];   /* a(n-2) for the next term */
        idx_t cand = 1;
        seq_t val;

        /* find the smallest unused integer coprime to prevprev */
        for (;;) {
            ensure_used_capacity(cand);
            if (!used[(size_t)cand] && gcd((seq_t)cand, prevprev) == (seq_t)1) {
                val = (seq_t)cand;
                break;
            }
            ++cand;
        }

        /* append the new term */
        if (seq_len == seq_cap) {
            seq_cap *= 2;
            seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
        }
        seq[seq_len] = val;
        ++seq_len;

        /* mark the chosen number as used */
        used[(size_t)cand] = true;
    }

    return seq[(size_t)(n - 1)];
}
