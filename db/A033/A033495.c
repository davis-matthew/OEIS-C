/*
 * A033495  Initial number for record sum of numbers in trajectory of 3x+1
 *          problem.
 * https://oeis.org/A033495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 39, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the initial numbers that set new record sums for their
 * 3x+1 (Collatz) trajectories.  a(0)=1, a(1)=2, a(2)=3, a(3)=6, ...
 * The function maintains a static list of record‑setting starting
 * numbers and, when requested for a larger index, scans successive n
 * until enough new records have been found. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *rec_numbers = NULL;     /* recorded starting numbers   */
    static seq_t *rec_sums    = NULL;     /* corresponding trajectory sums */
    static size_t rec_len = 0;            /* number of records found     */
    static size_t rec_cap = 0;            /* allocated capacity          */
    static idx_t next_start = 1;          /* next starting number to test*/
    static seq_t max_sum = 0;             /* highest sum seen so far    */

    /* Return a previously found record if the requested index is
     * already satisfied. */
    if ((size_t)n < rec_len)
        return rec_numbers[(size_t)n];

    /* Extend the record list until we have a(n). */
    while (rec_len <= (size_t)n) {
        seq_t x   = (seq_t)next_start;
        seq_t sum = x;

        /* Compute the Collatz sum for the current starting number. */
        while (x != 1) {
            if (x & 1)    /* odd */
                x = 3 * x + 1;
            else          /* even */
                x >>= 1;
            sum += x;
        }

        /* Record this number if it creates a new record sum. */
        if (sum > max_sum) {
            if (rec_len >= rec_cap) {          /* grow arrays if needed */
                size_t new_cap = rec_cap ? rec_cap * 2 : 4;
                rec_numbers = (seq_t*)realloc(rec_numbers,
                                               new_cap * sizeof(seq_t));
                rec_sums    = (seq_t*)realloc(rec_sums,
                                               new_cap * sizeof(seq_t));
                rec_cap = new_cap;
            }
            rec_numbers[rec_len] = next_start;
            rec_sums[rec_len]    = sum;
            max_sum = sum;
            rec_len++;
        }
        next_start++;
    }

    return rec_numbers[(size_t)n];
}
