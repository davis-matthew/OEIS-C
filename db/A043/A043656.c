/*
 * A043656  Numbers whose base-12 representation has exactly 7 runs.
 * https://oeis.org/A043656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A043656: Numbers whose base-12 representation has exactly 7 runs
 * 
 * The function builds the sequence by enumerating natural numbers
 * and counting the runs in each base‑12 representation.  Results are
 * cached to answer arbitrary future calls in O(1) time after the
 * first required element has been found.
 */

#include <stdlib.h>

/* count the number of runs in the base-12 representation of x */
static int count_runs(seq_t x)
{
    int runs = 0;
    seq_t last = (seq_t)12;        /* sentinel value larger than any digit (0–11) */
    while (x > 0) {
        seq_t d = x % (seq_t)12;   /* current least significant digit */
        if (d != last) {
            last = d;
            ++runs;
        }
        x /= (seq_t)12;            /* shift right by one digit */
    }
    return runs;
}

/* compute the nth term (1‑based index) of A043656 */
seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *seq = NULL;
    static size_t sz  = 0;   /* number of terms stored */
    static size_t cap = 0;   /* current capacity of the cache */
    static seq_t current = 0; /* last integer examined */

    /* ensure the cache can hold at least n terms */
    while (sz < n) {
        /* grow the cache if needed */
        if (seq == NULL) {
            cap = 1024;
            seq = (seq_t *)malloc(cap * sizeof(seq_t));
        } else if (sz == cap) {
            cap *= 2;
            seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
        }

        /* advance to the next integer */
        if (current == 0)
            current = 1;
        else
            ++current;

        /* record the integer if it has exactly 7 runs */
        if (count_runs(current) == 7) {
            seq[sz++] = current;
        }
    }
    return seq[n - 1];
}
