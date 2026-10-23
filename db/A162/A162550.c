/*
 * A162550  2n repeated C_n times, where C_n = A000108(n) is a Catalan number.
 * https://oeis.org/A162550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*k where k is the smallest integer with sum_{i=0}^k C_i
 *            > n, C_i = A000108(i) (Catalan numbers). The sequence lists each
 *            even number 2*k repeated C_k times, starting with k = 0
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static seq_t *C = NULL;      /* Catalan numbers */
static seq_t *cum = NULL;   /* Cumulative sums of Catalan numbers */
static idx_t len = 0;        /* Number of entries computed (equals cap of arrays) */
static size_t cap = 0;      /* Current capacity of arrays */

/* Ensure capacity for at least 'need' entries. */
static void ensure_capacity(idx_t need)
{
    if (cap < need) {
        size_t new_cap = cap == 0 ? 4 : cap * 2;
        if (new_cap < (size_t)need) new_cap = (size_t)need;
        seq_t *newC = (seq_t *)malloc(new_cap * sizeof(seq_t));
        seq_t *newcum = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (C && len > 0) memcpy(newC, C, len * sizeof(seq_t));
        if (cum && len > 0) memcpy(newcum, cum, len * sizeof(seq_t));
        if (C) free(C);
        if (cum) free(cum);
        C = newC;
        cum = newcum;
        cap = new_cap;
    }
}

/* Add the next Catalan number (index = len) and its cumulative sum. */
static void extend(void)
{
    idx_t i = len - 1;               /* index of last computed Catalan number */
    seq_t prev = C[i];
    seq_t numerator = prev * (seq_t)(2 * (2 * i + 1));
    seq_t next = numerator / (seq_t)(i + 2);
    C[len] = next;
    cum[len] = cum[len - 1] + next;
    len++;
}

seq_t a(idx_t n)
{
    /* Initial construction of C[0] = 1, cum[0] = 1. */
    if (len == 0) {
        ensure_capacity(4);
        C[0] = (seq_t)1;
        cum[0] = (seq_t)1;
        len = 1;
    }

    /* Extend the tables until the cumulative sum exceeds n. */
    while (cum[len - 1] <= n) {
        ensure_capacity(len + 1);
        if (len == 1) {            /* already have C[0] */
            /* Compute C[1] from C[0] */
            C[1] = (seq_t)1;
            cum[1] = cum[0] + C[1];
            len = 2;
        } else {
            extend();
        }
    }

    /* Find the minimal k with cum[k] > n. */
    idx_t k = 0;
    while (cum[k] <= n) k++;

    /* Return the term 2*k. */
    return (seq_t)2 * (seq_t)k;
}
