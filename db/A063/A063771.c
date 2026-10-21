/*
 * A063771  Numbers n such that n^sin(n) > m^sin(m) for all m < n and n^sin(n)
 *          is rounded to nearest integer.
 * https://oeis.org/A063771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 728, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A063771: n such that round(n^sin(n)) is a new record.
   We incrementally build the list of record‑breaking n.
   The sequence is defined by the condition:
       a(n) = smallest integer m > a(n−1) with
              round(m^sin(m)) > max{round(k^sin(k)) : k < m}
   and a(1)=1.  The function returns the n‑th record value.  */

#include <math.h>
#include <stdlib.h>
#include <stdint.h>

/* The maximum rounded value encountered so far. */
static seq_t max_val = 0;
/* Array of the record indices (stored as seq_t).  */
static seq_t *records = NULL;
/* Current number of records stored.  */
static size_t rec_len = 0;
/* Current capacity of the records array.  */
static size_t rec_cap = 0;
/* Next n to examine.  */
static idx_t next_n = 1;

/* Ensure that the records array has at least new_cap elements.  */
static void ensure_capacity(size_t new_cap)
{
    if (new_cap <= rec_cap)
        return;

    /* Allocate new larger array.  */
    seq_t *tmp = (seq_t *)realloc(records, new_cap * sizeof(seq_t));
    if (!tmp) { /* Allocation failure: abort.  */
        abort();
    }
    records = tmp;
    rec_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Allocate initial space if necessary.  */
    if (rec_cap == 0) {
        rec_cap = 128;
        records = (seq_t *)malloc(rec_cap * sizeof(seq_t));
        if (!records) abort();
    }

    /* Generate records until we have at least n of them. */
    while (rec_len < n) {
        /* Make sure we have room for another record entry. */
        if (rec_len + 1 > rec_cap)
            ensure_capacity(rec_cap * 2);

        double nd = (double)next_n;
        double val = pow(nd, sin(nd));
        seq_t round_val = (seq_t)llround(val);

        if (round_val > max_val) {          /* New record found.      */
            max_val = round_val;
            records[rec_len++] = (seq_t)next_n;
        }
        ++next_n;
    }

    return records[n - 1];
}
