/*
 * A080452  a(1) = 8, a(n) = smallest (nontrivial) multiple of a(n-1)
 *          containing n digits, a(n) not equal to 10*a(n-1).
 * https://oeis.org/A080452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest (nontrivial) multiple of a(n-1) with exactly n
 *            decimal digits, * and not equal to 10*a(n-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/mem.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void seq_free_array(seq_t* p)          { mem_free(p); }

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;              /* undefined for n < 1 */

    static seq_t *seq = NULL;                /* memoized values a(1)..a(len) */
    static size_t len = 0;                   /* number of stored terms   */

    /* initialisation: a(1) = 8 */
    if (len == 0) {
        seq = seq_alloc_new_array(1);
        seq[0] = (seq_t)8;
        len = 1;
    }

    /* already computed? */
    if ((size_t)n <= len)
        return seq[n-1];

    /* grow storage to size n */
    seq_t *new_seq = seq_alloc_new_array(n);
    for (size_t i = 0; i < len; ++i)
        new_seq[i] = seq[i];
    seq_free_array(seq);
    seq = new_seq;

    /* compute missing terms */
    for (size_t i = len; i < n; ++i) {
        idx_t idx = (idx_t)(i + 1);          /* current n (1‑based)          */
        seq_t prev = seq[idx-2];             /* a(idx-1)                     */
        seq_t low  = pow10(idx-1);          /* 10^(n-1)                     */
        seq_t m    = (low + prev - (seq_t)1) / prev;  /* ceil division */
        if (m < (seq_t)2) m = (seq_t)2;     /* ensure non‑trivial multiple  */
        if (m == (seq_t)10) m = (seq_t)11;  /* skip the forbidden 10×a(idx-1) */
        seq[idx-1] = m * prev;              /* a(idx) = m * a(idx-1)        */
    }
    len = n;
    return seq[n-1];
}
