/*
 * A130111  Rearrangement of positive integers such that each five terms sum
 *          up to a perfect square.
 * https://oeis.org/A130111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A130111 – Rearrangement of positive integers into 5‑column rows
 *  such that each row sum is a perfect square.
 *
 *  a(n) returns the nth term of the sequence (n = 1,2,3,…).
 *
 *  The algorithm builds rows on demand, storing already computed
 *  terms in a static array and keeping a bitmap of numbers that
 *  have already appeared.  Each call is O(n) in total and works
 *  regardless of the order of the indices requested.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/floor_sqrt.h"   /* provides seq_t floor_sqrt(seq_t) */

/* Public function required by the assignment */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 */

    /* Static state */
    static seq_t *seq   = NULL;   /* array of computed sequence values   */
    static size_t seq_len = 0;    /* number of elements stored          */
    static size_t seq_cap = 0;    /* capacity of seq array              */

    static unsigned char *used   = NULL;  /* 0=unused,1=used for integers   */
    static size_t used_cap   = 0;         /* capacity of used array          */

    static seq_t cand = 1;        /* next integer to consider          */

    /* Ensure seq buffer capacity */
    if (seq_len == 0) {
        seq_cap = 64;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    } else if (seq_len + 5 > seq_cap) {   /* need room for one more row */
        seq_cap <<= 1;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
    }

    /* Ensure used array capacity up to current candidate */
    while ((size_t)cand > used_cap) {
        size_t newcap = (used_cap ? used_cap << 1 : 64);
        used = (unsigned char *)realloc(used, newcap * sizeof(unsigned char));
        memset(used + used_cap, 0, (newcap - used_cap) * sizeof(unsigned char));
        used_cap = newcap;
    }

    /* Convert n to zero‑based target index */
    size_t target = (size_t)n - 1;
    if (target < seq_len) return seq[target];

    /* Build rows until we have computed up to target */
    while (seq_len <= target) {
        seq_t row[5];
        seq_t sum = 0;

        /* Pick first four entries: smallest unused integers */
        for (int i = 0; i < 4; ++i) {
            /* Find next unused integer */
            while (used[(size_t)cand]) ++cand;
            /* Ensure used array can hold cand (already done above) */
            row[i] = cand;
            used[(size_t)cand] = 1;
            sum += row[i];
            ++cand;
        }

        /* Find fifth entry: smallest unused integer > last4
           that makes the row sum a perfect square. */
        seq_t cand5 = row[3] + 1;
        for (;; ++cand5) {
            /* Ensure used array can hold cand5 */
            while ((size_t)cand5 > used_cap) {
                size_t newcap = (used_cap ? used_cap << 1 : 64);
                used = (unsigned char *)realloc(used, newcap * sizeof(unsigned char));
                memset(used + used_cap, 0, (newcap - used_cap) * sizeof(unsigned char));
                used_cap = newcap;
            }

            if (!used[(size_t)cand5]) {
                seq_t total = sum + cand5;
                seq_t root  = floor_sqrt(total);
                if (root * root == total) break;
            }
        }

        row[4] = cand5;
        used[(size_t)cand5] = 1;
        /* If cand5 was the next integer we hadn't used yet, advance cand */
        if (cand5 == cand) ++cand;

        /* Store completed row */
        for (int i = 0; i < 5; ++i) seq[seq_len++] = row[i];
    }

    return seq[target];
}
