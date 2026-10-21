/*
 * A079041  Numbers n in which the first k digits of n form an integer
 *          divisible by k^3, for k = 1, 2, ..., m, where m is the number of
 *          digits in n.
 * https://oeis.org/A079041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A079041: Numbers n where the first k digits of n form an integer divisible by k^3, for all k=1..m (m=number of digits of n). */
/* Implementation: generate all such numbers by depth‑first search on digit prefixes, keeping them in ascending order by length. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *terms = NULL;          /* array of found terms      */
static size_t terms_size = 0;        /* how many terms stored    */
static size_t terms_cap  = 0;        /* allocated capacity       */
static int    current_len = 0;       /* longest length processed */

/* Ensure the terms array can hold at least `min_cap` elements. */
static void grow_terms(size_t min_cap)
{
    if (terms_cap >= min_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);   /* memory exhaustion */
    terms = tmp;
    terms_cap = new_cap;
}

/* Recursively build all numbers of given `len` that satisfy the prefix‑divisibility property. */
static void dfs(int depth, int len, seq_t pref)
{
    if (depth == len) {                 /* full length reached */
        if (terms_size >= terms_cap) grow_terms(terms_size + 1);
        terms[terms_size++] = pref;
        return;
    }

    int start = (depth == 0) ? 1 : 0;    /* first digit cannot be 0 */
    for (int d = start; d <= 9; ++d) {
        seq_t new_pref = pref * (seq_t)10 + (seq_t)d;
        int k = depth + 1;
        int cube = k * k * k;            /* k^3 fits in int for all lengths encountered */
        if (new_pref % (seq_t)cube == 0) {
            dfs(depth + 1, len, new_pref);
        }
    }
}

/* Main interface required by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n==0 */

    /* Generate terms until we have at least n of them. */
    if (terms_size < (size_t)n) {
        for (int len = current_len + 1; terms_size < (size_t)n; ++len) {
            dfs(0, len, (seq_t)0);
            current_len = len;
        }
    }

    return terms[n - 1];
}
