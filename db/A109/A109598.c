/*
 * A109598  n is in this sequence if for all m > 0, n+m is not divisible by
 *          6m-1, 6m+1, 8m-1 or 8m+1.
 * https://oeis.org/A109598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   This code implements the definition of OEIS A109598. * A number
 *            n is in the sequence if for all positive integers m * n+m is not
 *            divisible by 6m-1, 6m+1, 8m-1, or 8m+1. * The function generates
 *            the sequence on demand, caching all terms * found so far. It
 *            returns the n‑th term (1‑based index)
 * Range:     u8 n <= 31, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   good_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/good_candidate.h"

/* static state for memoisation */
static seq_t *vals = NULL;    /* array of found sequence elements */
static size_t val_cap = 0;    /* capacity of vals */
static idx_t  val_cnt = 0;    /* number of elements stored */
static idx_t  cur_n   = 1;    /* next integer to test */

/* ensure the array has space for at least new_cap elements */
static void ensure_cap(size_t new_cap)
{
    if (new_cap <= val_cap) return;
    size_t cap = val_cap ? val_cap * 2 : 64;
    if (cap < new_cap) cap = new_cap;
    seq_t *tmp = (seq_t*)realloc(vals, cap * sizeof(seq_t));
    vals = tmp;
    val_cap = cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for 0, return 0 */

    /* generate terms until we have n of them */
    while (val_cnt < n) {
        if (good_candidate(cur_n)) {
            ensure_cap(val_cnt + 1);
            vals[val_cnt++] = (seq_t)cur_n;
        }
        ++cur_n;
    }
    return vals[n - 1];
}
