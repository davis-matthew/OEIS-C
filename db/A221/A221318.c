/*
 * A221318  Number of nonnegative integer arrays of length n summing to n
 *          without equal adjacent values modulo 5.
 * https://oeis.org/A221318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 50, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: number of nonnegative integer arrays of length n
   summing to n with no equal adjacent values modulo 5.
   For each n we count residue sequences (mod 5) with no adjacent equal
   residues, then multiply by the number of weak compositions of the
   remaining sum.  The function memoises all intermediate results
   so it can be called in any order. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* binomial coefficient using seq_t arithmetic */
static seq_t binom_seq(idx_t a, idx_t b)
{
    if (b > a - b) b = a - b;
    seq_t res = 1;
    for (idx_t i = 1; i <= b; ++i) {
        res = res * (seq_t)(a - b + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* memoisation storage */
    static idx_t          dp_max_n   = 0;
    static seq_t        *res        = NULL;
    static size_t          res_cap   = 0;
    static seq_t        *cur[5]    = {NULL, NULL, NULL, NULL, NULL};
    static seq_t        *nxt[5]    = {NULL, NULL, NULL, NULL, NULL};
    static size_t          cur_cap   = 0;       /* max residue sum + 1 */

    /* enlarge result cache if necessary */
    if (res_cap <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(res, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        res = tmp;
        for (size_t i = res_cap; i < new_cap; ++i) res[i] = 0;
        res_cap = new_cap;
    }

    /* return immediately if already computed */
    if (n <= dp_max_n) return res[n];

    /* determine needed size for residue‑sum arrays */
    size_t needed_cap = n * 4 + 1;   /* maximum possible sum of residues */

    /* grow residue‑sum arrays if required */
    if (cur_cap < needed_cap) {
        for (int r = 0; r < 5; ++r) {
            seq_t *tmp = (seq_t *)realloc(cur[r], needed_cap * sizeof(seq_t));
            if (tmp == NULL) abort();
            cur[r] = tmp;
            memset(cur[r] + cur_cap, 0,
                   (needed_cap - cur_cap) * sizeof(seq_t));
        }
        for (int r = 0; r < 5; ++r) {
            if (nxt[r] == NULL) {
                nxt[r] = (seq_t *)calloc(needed_cap, sizeof(seq_t));
                if (nxt[r] == NULL) abort();
            } else {
                seq_t *tmp = (seq_t *)realloc(nxt[r], needed_cap * sizeof(seq_t));
                if (tmp == NULL) abort();
                nxt[r] = tmp;
                memset(nxt[r], 0, needed_cap * sizeof(seq_t));
            }
        }
        cur_cap = needed_cap;
    }

    /* initialise for n == 1 if this is the first call */
    if (dp_max_n == 0) {
        for (int r = 0; r < 5; ++r)
            memset(cur[r], 0, cur_cap * sizeof(seq_t));
        for (int r = 0; r < 5; ++r)
            cur[r][r] = 1;               /* sequences of length 1 */
        res[1] = 1;
        dp_max_n = 1;
    }

    size_t cur_max_s = dp_max_n * 4;  /* current maximum residue sum */

    /* build up from the current maximum length to the required n */
    for (idx_t len = dp_max_n + 1; len <= n; ++len) {
        size_t prev_max_s = cur_max_s;
        size_t new_max_s  = len * 4;

        /* reset temporary arrays */
        for (int r = 0; r < 5; ++r) {
            memset(nxt[r], 0,
                   (new_max_s + 1) * sizeof(seq_t));
        }

        /* transition from length len-1 to len */
        for (size_t s = 0; s <= prev_max_s; ++s) {
            seq_t total = cur[0][s] + cur[1][s] + cur[2][s] + cur[3][s] + cur[4][s];
            for (int r = 0; r < 5; ++r) {
                seq_t val = total - cur[r][s];
                size_t dest = s + (size_t)r;
                nxt[r][dest] += val;
            }
        }

        /* swap current and next arrays */
        for (int r = 0; r < 5; ++r) {
            seq_t *tmp = cur[r];
            cur[r] = nxt[r];
            nxt[r] = tmp;
        }
        cur_max_s = new_max_s;

        /* compute a(len) from the residue counts */
        seq_t total_val = 0;
        for (size_t s = 0; s <= cur_max_s; ++s) {
            if (s > (size_t)len) continue;                 /* negative k would follow */
            if (((size_t)len - s) % 5 != 0) continue;       /* not divisible by 5   */
            seq_t fsum = cur[0][s] + cur[1][s] + cur[2][s] + cur[3][s] + cur[4][s];
            idx_t k = ((idx_t)len - (idx_t)s) / 5;
            seq_t w = binom_seq(k + len - 1, len - 1);
            total_val += fsum * w;
        }
        res[len] = total_val;
        dp_max_n = len;
    }

    return res[n];
}
