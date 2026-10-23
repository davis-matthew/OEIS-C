/*
 * A180176  Lexicographically earliest permutation of the natural numbers such
 *          that a(n) != n and in decimal representation a(n) and n have at
 *          least one common digit.
 * https://oeis.org/A180176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   shares_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically earliest involutive permutation of natural numbers
 * such that a(n) != n and n and a(n) share at least one decimal digit.
 * Implemented by a greedy construction: when the smallest unassigned
 * index n is processed, it is paired with the smallest unused number
 * m > n that shares a digit with n.  The process yields the unique
 * permutation described in OEIS A180176. */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/shares_digit.h"

/* state kept between calls */
static seq_t *perm = NULL;           /* array of values a(i)   */
static bool   *assigned = NULL;      /* true if perm[i] set    */
static size_t  perm_cap = 0;         /* current capacity       */

/* ensure that indices up to 'need' (inclusive) are valid */
static void ensure_capacity(idx_t need)
{
    size_t need_cap = (size_t)need + 1;
    if (need_cap <= perm_cap) return;
    size_t new_cap = perm_cap ? perm_cap * 2 : 4;
    while (new_cap < need_cap) new_cap <<= 1;
    perm    = (seq_t *)realloc(perm,    new_cap * sizeof(seq_t));
    assigned= (bool  *)realloc(assigned,new_cap * sizeof(bool));
    memset(perm + perm_cap, 0,        (new_cap - perm_cap) * sizeof(seq_t));
    memset(assigned + perm_cap, 0,     (new_cap - perm_cap) * sizeof(bool));
    perm_cap = new_cap;
}

/* main sequence routine */
seq_t a(idx_t n)
{
    /* initialise on first call */
    if (!perm) ensure_capacity(16);

    /* process all indices up to n in order of increasing smallest
       unassigned number */
    idx_t cur = 0;
    while (true) {
        while (cur < perm_cap && assigned[cur]) cur++;
        if (cur > n) break;      /* all indices ≤ n are assigned */

        ensure_capacity(cur);
        /* find partner m > cur */
        idx_t m = cur + 1;
        while (true) {
            ensure_capacity(m);
            if (!assigned[m] && shares_digit(cur, m)) break;
            m++;
        }
        /* store the pair */
        perm[cur] = (seq_t)m;
        perm[m]   = (seq_t)cur;
        assigned[cur] = true;
        assigned[m]   = true;
        cur++;                 /* next smallest candidate */
    }
    return perm[n];
}
