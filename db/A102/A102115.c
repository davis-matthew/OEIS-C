/*
 * A102115  Iccanobirt numbers (5 of 15): a(n) = R(a(n-1)) + a(n-2) +
 *          R(a(n-3)), where R is the digit reversal function A004086.
 * https://oeis.org/A102115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = R(a(n-1)) + a(n-2) + R(a(n-3)) , a(0)=0,a(1)=0,a(2)=1
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 29, u64 n <= 54, u128 n <= 106,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 870 (2026-11-03)
 * Helpers:   mem, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/reverse_digits.h"
#include "../utils/mem.h"

/* helper to allocate an array of seq_t that works in both C and C++ */
static inline seq_t *seq_malloc(idx_t n)
{
#ifdef __cplusplus
    return mem_new(seq_t, n);
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

static inline seq_t *seq_realloc(seq_t *p, idx_t n)
{
#ifdef __cplusplus
    return mem_resize(seq_t, p, n);
#else
    return (seq_t *)realloc(p, n * sizeof(seq_t));
#endif
}

/* static array for memoisation */
static seq_t *seq = NULL;
static idx_t cur_max = 0;

seq_t a(idx_t n)
{
    /* initialise the first three values on first call */
    if (seq == NULL) {
        seq = seq_malloc(3);          /* allocate indices 0,1,2 */
        seq[0] = 0;
        seq[1] = 0;
        seq[2] = 1;
        cur_max = 2;                  /* we have computed up to index 2 */
    }

    /* if the desired term is already computed, return it */
    if (n <= cur_max)
        return seq[n];

    /* grow the table to hold indices up to n */
    idx_t old_max = cur_max;
    seq = seq_realloc(seq, n + 1);
    cur_max = n;

    /* compute new terms using the recurrence */
    for (idx_t i = old_max + 1; i <= n; ++i) {
        seq_t rev_prev   = reverse_digits(seq[i - 1]); /* R(a(i-1)) */
        seq_t rev_prev3  = reverse_digits(seq[i - 3]); /* R(a(i-3)) */
        seq[i] = rev_prev + seq[i - 2] + rev_prev3;
    }

    return seq[n];
}
