/*
 * A180348  Lexicographically earliest sequence such that a(n) is coprime to
 *          the preceding 4 terms and does not occur earlier; a(n) = n for
 *          n=1,2,3,4.
 * https://oeis.org/A180348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lexicographically earliest sequence such that a(n) is
 *            coprime to the preceding 4 terms and does not occur earlier;
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=4
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/gcd.h"

/* Static storage for the sequence and used values */
static seq_t *seq      = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;

static bool *used      = NULL;
static size_t used_cap = 0;
static size_t min_unused = 1;   /* smallest integer not yet used   */

/* Ensure the sequence array has at least 'need' capacity */
static void ensure_seq_cap(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < need) new_cap *= 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!seq) exit(EXIT_FAILURE);
    seq_cap = new_cap;
}

/* Ensure the used bit‑array has at least 'need' capacity */
static void ensure_used_cap(size_t need)
{
    if (used_cap > need) return;
    size_t new_cap = used_cap ? used_cap * 2 : 8;
    while (new_cap <= need) new_cap <<= 1;
    bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!tmp) exit(EXIT_FAILURE);
    if (new_cap > used_cap)
        memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
    used = tmp;
    used_cap = new_cap;
}

/* Return the nth term, building the sequence on demand. */
seq_t a(idx_t n)
{
    if (n <= 0) return (seq_t)0;   /* undefined for n<1 */

    /* Build sequence up to index n (1‑based). */
    while (seq_len < (size_t)n) {
        if (seq_len < 4) {
            /* First four terms are fixed */
            seq_t val = (seq_t)(seq_len + 1);   /* 1‑based value */
            ensure_seq_cap(seq_len + 1);
            seq[seq_len] = val;
            /* Mark used */
            ensure_used_cap((size_t)val);
            used[(size_t)val] = true;
            /* Advance min_unused */
            ensure_used_cap(min_unused);
            while (used[min_unused]) ++min_unused;
            ++seq_len;
            continue;
        }

        /* Find the smallest unused integer coprime with the last four terms */
        seq_t cand = (seq_t)min_unused;
        for (;;) {
            /* Ensure we can index the used array */
            ensure_used_cap((size_t)cand);
            if (used[(size_t)cand]) {   /* already used */
                ++cand;
                continue;
            }
            seq_t a1 = seq[seq_len - 1];
            seq_t a2 = seq[seq_len - 2];
            seq_t a3 = seq[seq_len - 3];
            seq_t a4 = seq[seq_len - 4];
            if (gcd(a1, cand) == 1 &&
                gcd(a2, cand) == 1 &&
                gcd(a3, cand) == 1 &&
                gcd(a4, cand) == 1) {
                /* Accept this candidate */
                ensure_seq_cap(seq_len + 1);
                seq[seq_len] = cand;
                used[(size_t)cand] = true;
                /* Update min_unused */
                ensure_used_cap(min_unused);
                while (used[min_unused]) ++min_unused;
                ++seq_len;
                break;
            }
            ++cand;
        }
    }

    return seq[n - 1];
}
