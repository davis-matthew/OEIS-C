/*
 * A071562  Numbers n such that the sum of the middle divisors of n (A071090)
 *          is not zero.
 * https://oeis.org/A071562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_property
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the nth integer n>0 such that n has a divisor a with
   a ≤ n/a ≤ 2·a.  The function builds the sequence on demand by testing
   successive integers. */
/* Note: a(1)=1, a(2)=2, a(3)=4, a(4)=6, … (OEIS A071562) */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_property.h"

/* Dynamic array holding the sequence values seen so far. */
static seq_t *seq_arr  = NULL;   /* array of seq_t values */
static idx_t seq_len   = 0;      /* number of values stored */
static size_t seq_cap = 0;       /* current capacity of seq_arr */
static seq_t next_test = 1;      /* next integer to test for inclusion */

/* Ensure the dynamic array has at least new_capacity elements. */
static void ensure_capacity(size_t new_capacity) {
    if (new_capacity > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 16;
        while (new_cap < new_capacity)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_arr = tmp;
        seq_cap = new_cap;
    }
}

/* Append a value to the dynamic array, expanding it if necessary. */
static void append_value(seq_t val) {
    ensure_capacity(seq_len + 1);
    seq_arr[seq_len++] = val;
}

/* Return the nth term of A071562 (n≥1).  The function keeps the
   generated sequence in static storage and extends it lazily. */
seq_t a(idx_t n) {
    if (n == 0)            /* undefined for n=0; return 0 */
        return (seq_t)0;

    /* Generate terms until we have at least n of them. */
    while (seq_len < n) {
        if (has_property(next_test))
            append_value(next_test);
        ++next_test;
    }

    return seq_arr[n - 1];
}
