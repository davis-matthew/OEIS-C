/*
 * A102829  "True already", base 3, start 1: a(n) is the least integer such
 *          that the sequence up to a(n-1) written in base 3 contains
 *          floor(a(n)/3) copies of the digit a(n) % 3, with a(0) = 1.
 * https://oeis.org/A102829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the least integer x such that the base‑3 representation
 *            of the sequence a(0),a(1),…,a(n‑1) contains *exactly* floor(x/3)
 *            copies of the digit (x mod 3). With a(0)=1. The code maintains
 *            the digit counts of the already computed terms and finds the
 *            next term by a simple linear scan
 * Range:     u8 n <= 48, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   inc_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/inc_counts.h"

/* Static cache of computed terms and digit counts. */
static seq_t *seq      = NULL;   /* a(0)…a(n) */
static size_t seq_cap  = 0;      /* array capacity */
static size_t seq_len  = 0;      /* number of filled terms */
static seq_t digit_cnt[3] = {0,0,0};  /* digits 0,1,2 counts in all terms so far */

seq_t a(idx_t n)
{
    /* Initialise on the first call. */
    if (seq_len == 0)
    {
        seq_cap = 4;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)1;              /* a(0) = 1 */
        inc_counts(seq[0], digit_cnt);
        seq_len = 1;
    }

    /* Extend the cache up to index n. */
    while (seq_len <= (size_t)n)
    {
        /* Grow array if necessary. */
        if (seq_len >= seq_cap)
        {
            size_t new_cap = seq_cap ? seq_cap * 2 : 1;
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            seq = tmp;
            seq_cap = new_cap;
        }

        seq_t prev = seq[seq_len - 1];
        seq_t candidate = prev + (seq_t)1;

        /* Find the least integer satisfying the equality condition. */
        while (true)
        {
            size_t digit = (size_t)(candidate % 3);
            seq_t required = candidate / 3;      /* floor(candidate/3) */
            if (digit_cnt[digit] == required)
                break;
            candidate += (seq_t)1;
        }

        seq[seq_len] = candidate;
        seq_len++;
        inc_counts(candidate, digit_cnt);
    }

    return seq[(size_t)n];
}
