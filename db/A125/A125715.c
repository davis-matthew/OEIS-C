/*
 * A125715  a(1)=1. a(n) = the smallest positive integer not occurring earlier
 *          in the sequence such that (sum{k=1 to n-1} a(k)) is congruent to
 *          a(n) (mod n).
 * https://oeis.org/A125715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n≥2, a(n) = the smallest positive integer not yet
 *            used such that (∑_{k=1}^{n-1} a(k)) ≡ a(n) (mod n). The function
 *            builds the sequence on demand, memoising all values and keeping
 *            a table of which integers have already appeared
 * Range:     u8 n <= 19, u16 n <= 422, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *seq      = NULL;
    static size_t seq_cap  = 0;
    static size_t seq_len  = 0;

    /* Boolean table of used values.  Index i corresponds to integer i. */
    static uint8_t *used    = NULL;
    static size_t used_cap  = 0;

    /* Running sum of all terms computed so far */
    static seq_t sum_so_far = 0;

    /* Return cached value if already computed */
    if (n <= seq_len)
        return seq[n-1];

    /* Grow the terms cache if necessary */
    if (seq_cap < (size_t)n) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        while (new_cap < n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* OOM: abort */
        seq     = tmp;
        seq_cap = new_cap;
    }

    /* Compute terms from seq_len+1 up to n (1‑based indices) */
    for (size_t i = seq_len; i < (size_t)n; ++i) {
        idx_t k = (idx_t)(i + 1);              /* current index (n≥1) */
        seq_t rem = sum_so_far % (seq_t)k;     /* remainder condition */

        /* Start candidate at k if remainder is 0, otherwise at remainder */
        seq_t cand = (rem == 0) ? (seq_t)k : rem;

        /* Find the smallest unused integer with the required residue */
        for (;;) {
            idx_t idx = (idx_t)cand;           /* index into used table */

            /* Grow used table to cover idx if needed */
            if (idx >= used_cap) {
                size_t new_used_cap = used_cap ? used_cap * 2 : 16;
                while (new_used_cap <= idx)
                    new_used_cap *= 2;
                uint8_t *tmp = (uint8_t *)realloc(used,
                                                   new_used_cap * sizeof(uint8_t));
                if (!tmp) abort();          /* OOM */
                /* Zero‑initialize newly allocated part */
                if (new_used_cap > used_cap)
                    memset(tmp + used_cap, 0,
                           (new_used_cap - used_cap) * sizeof(uint8_t));
                used     = tmp;
                used_cap = new_used_cap;
            }

            if (!used[idx]) {                 /* not used yet */
                used[idx] = 1;                /* mark as used */
                break;
            }
            cand += k;                         /* try next in residue class */
        }

        seq[i]      = cand;                    /* store new term */
        seq_len++;                             /* update length */
        sum_so_far += cand;                    /* update running sum */
    }

    return seq[n-1];
}
