/*
 * A033494  Record sum of numbers in trajectory of 3x+1 problem.
 * https://oeis.org/A033494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   collatz_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A033494: Record sum of numbers in 3x+1 trajectory.
   a(n) is the n‑th record sum of the Collatz trajectory sums. */

#include <stdlib.h>
#include "../utils/collatz_sum.h"

static seq_t *records = NULL;   /* array of record sums */
static size_t rec_cap = 0;      /* allocated capacity */
static size_t rec_len = 0;      /* number of records stored */
static seq_t max_sum = 0;       /* current maximum sum */
static idx_t  next_start = 1;   /* next starting integer to test */

/* Grow the record array to hold at least 'needed' elements. */
static void grow_records(size_t needed)
{
    if (rec_cap < needed) {
        size_t new_cap = rec_cap ? rec_cap << 1 : 8;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(records, new_cap * sizeof(seq_t));
        records = tmp;
        rec_cap = new_cap;
    }
}

/* Return the n-th record sum (0‑based index). */
seq_t a(idx_t n)
{
    if (!records) {
        rec_cap  = 8;
        records  = (seq_t*)malloc(rec_cap * sizeof(seq_t));
        rec_len  = 0;
        max_sum  = 0;
        next_start = 1;
    }

    while (rec_len <= n) {
        seq_t s = collatz_sum(next_start);
        if (s > max_sum) {
            grow_records(rec_len + 1);
            records[rec_len++] = s;
            max_sum = s;
        }
        next_start++;
    }
    return records[n];
}
