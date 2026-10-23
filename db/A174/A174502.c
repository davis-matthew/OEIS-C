/*
 * A174502  Continued fraction expansion for exp( Sum_{n>=1} 1/(n*A086903(n))
 *          ), where A086903(n) = (4+sqrt(15))^n + (4-sqrt(15))^n.
 * https://oeis.org/A174502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cofr, nonn, easy
 * Formula:   a(0)=1, a(1)=6, a(2)=1, a(3)=60, a(4)=1, a(5)=486, for n>=6,
 *            a(n)=9*a(n-2)-9*a(n-4)+a(n-6)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static seq_t *seq = NULL;          /* array of sequence values */
static idx_t  seq_cap = 0;         /* allocated capacity */
static idx_t  seq_computed = 0;    /* number of entries that have been filled */

/* Memoized computation of A174502. */
seq_t a(idx_t n)
{
    /* Grow the array if necessary. */
    if (n >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;

        seq_t *new_arr = seq_alloc_new_array(new_cap);

        /* Copy existing values, if any. */
        for (idx_t i = 0; i < seq_computed; ++i)
            new_arr[i] = seq[i];

        /* Free old storage. */
        if (seq != NULL)
            seq_free_array(seq);

        seq      = new_arr;
        seq_cap  = new_cap;
    }

    /* Compute missing terms up to index n. */
    for (idx_t i = seq_computed; i <= n; ++i) {
        if (i == 0)          seq[i] = 1;
        else if (i == 1)     seq[i] = 6;
        else if (i == 2)     seq[i] = 1;
        else if (i == 3)     seq[i] = 60;
        else if (i == 4)     seq[i] = 1;
        else if (i == 5)     seq[i] = 486;
        else {
            seq_t val = 9 * seq[i - 2];     /* 9*a(i-2) */
            val -= 9 * seq[i - 4];          /* -9*a(i-4) */
            val += seq[i - 6];              /* +a(i-6)   */
            seq[i] = val;
        }
    }
    seq_computed = n + 1;
    return seq[n];
}
