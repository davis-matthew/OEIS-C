/*
 * A102826  "True already", base 4, start 0: a(n) is the least integer such
 *          that the sequence up to a(n-1) written in base 4 contains
 *          floor(a(n)/4) copies of the digit a(n) % 4, with a(0) = 0.
 * https://oeis.org/A102826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(0) = 0; a(n) = the minimal integer k > a(n–1) such that the
 *            number of occurrences of digit d = k mod 4 in the base‑4
 *            representations of a(0)…a(n–1) equals floor(k/4). Equivalently,
 *            a(n) = min{ 4*cnt[d] + d | d = 0..3 and 4*cnt[d] + d > a(n–1) },
 *            where cnt[d] is the count of digit d among the ba...
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *seq = NULL;       /* cached sequence values */
static idx_t  seq_len = 0;      /* number of cached terms */
static uint64_t cnt[4] = {0};   /* counts of digits 0..3 in all terms so far */

/* add the base‑4 digits of v to the global count array */
static void add_digits(seq_t v)
{
    if (v == 0) {
        cnt[0]++;  /* representation of 0 is "0" */
        return;
    }
    while (v != 0) {
        cnt[(size_t)(v % 4)]++;
        v /= 4;
    }
}

seq_t a(idx_t n)
{
    /* initialise cache on first use */
    if (seq_len == 0) {
        seq = (seq_t *)malloc(sizeof(seq_t));
        if (!seq) abort();
        seq[0] = (seq_t)0;
        seq_len = 1;
        add_digits(seq[0]);          /* include a(0) in digit counts */
    }

    /* extend the sequence until it contains a(n) */
    while (seq_len <= n) {
        seq_t prev = seq[seq_len - 1];
        seq_t best = 0;
        bool first = true;

        /* examine the four possible digits */
        for (int d = 0; d < 4; d++) {
            seq_t cand = (seq_t)cnt[d] * (seq_t)4 + (seq_t)d;
            if (cand > prev) {
                if (first || cand < best) {
                    best = cand;
                    first = false;
                }
            }
        }

        /* store the next term and update digit counts */
        seq_len++;
        seq = (seq_t *)realloc(seq, sizeof(seq_t) * seq_len);
        if (!seq) abort();
        seq[seq_len - 1] = best;
        add_digits(best);
    }

    return seq[n];
}
