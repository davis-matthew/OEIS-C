/*
 * A169838  a(n) = position of n in A169837.
 * https://oeis.org/A169838
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of n in the permutation (A169837) defined by:
 *            start with 3, then repeatedly append the smallest unused integer
 *            that shares a prime factor with the previous term
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Only defined for n >= 2; behaviour for smaller n is irrelevant. */
    if (n < 2) return (seq_t)0;

    /* Static state: the generated permutation and a lookup of positions. */
    static seq_t *perm     = NULL;
    static size_t perm_cap = 0;
    static size_t perm_len = 0;

    static idx_t *pos     = NULL;
    static size_t pos_cap = 0;

    /* Initialise arrays if this is the first call. */
    if (perm_cap == 0) {
        perm_cap = 8;
        perm = (seq_t *)malloc(perm_cap * sizeof(seq_t));
    }
    if (pos_cap == 0) {
        pos_cap = 2048;          /* initial capacity large enough for small n */
        pos = (idx_t *)calloc(pos_cap, sizeof(idx_t));
    }

    /* Ensure the permutation has at least one term (3). */
    if (perm_len == 0) {
        perm[0] = (seq_t)3;
        perm_len = 1;
        /* Ensure the position table can hold index 3. */
        if ((size_t)3 >= pos_cap) {
            size_t new_cap = pos_cap * 2;
            while (new_cap <= (size_t)3) new_cap *= 2;
            idx_t *tmp = (idx_t *)realloc(pos, new_cap * sizeof(idx_t));
            memset(tmp + pos_cap, 0, (new_cap - pos_cap) * sizeof(idx_t));
            pos = tmp;
            pos_cap = new_cap;
        }
        pos[(idx_t)3] = 1;   /* positions are 1‑based */
    }

    /* If we already know the position of n, return it immediately. */
    if ((size_t)n < pos_cap && pos[(idx_t)n] != 0)
        return (seq_t)pos[(idx_t)n];

    /* Generate the permutation until n appears. */
    for (;;) {
        seq_t curr = perm[perm_len - 1];
        idx_t cand = 2;          /* smallest candidate to test */
        seq_t next = 0;

        /* Find the smallest unused integer sharing a prime factor with curr. */
        for (;;) {
            /* Grow position table if needed. */
            if ((size_t)cand >= pos_cap) {
                size_t new_cap = pos_cap * 2;
                while (new_cap <= (size_t)cand) new_cap *= 2;
                idx_t *tmp = (idx_t *)realloc(pos, new_cap * sizeof(idx_t));
                memset(tmp + pos_cap, 0, (new_cap - pos_cap) * sizeof(idx_t));
                pos = tmp;
                pos_cap = new_cap;
            }

            if (pos[(idx_t)cand] == 0) {     /* unused */
                if (gcd(curr, (seq_t)cand) != (seq_t)1) {
                    next = (seq_t)cand;
                    break;
                }
            }
            cand++;
        }

        /* Append the new term, expanding the permutation array if required. */
        if (perm_len == perm_cap) {
            perm_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(perm, perm_cap * sizeof(seq_t));
            perm = tmp;
        }
        perm[perm_len++] = next;

        /* Record its position, enlarging the table if necessary. */
        if ((size_t)next >= pos_cap) {
            size_t new_cap = pos_cap * 2;
            while (new_cap <= (size_t)next) new_cap *= 2;
            idx_t *tmp = (idx_t *)realloc(pos, new_cap * sizeof(idx_t));
            memset(tmp + pos_cap, 0, (new_cap - pos_cap) * sizeof(idx_t));
            pos = tmp;
            pos_cap = new_cap;
        }
        pos[(idx_t)next] = (idx_t)perm_len;   /* 1‑based position */

        if (next == (seq_t)n)
            return (seq_t)perm_len;
    }
}
