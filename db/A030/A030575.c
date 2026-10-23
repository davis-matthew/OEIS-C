/*
 * A030575  Run length of n-th run of digit 0 in A030567.
 * https://oeis.org/A030575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = run length of the n-th run of digit 0 in the
 *            concatenation of the reverse base‑6 representations of the
 *            non‑negative integers
 * Range:     u8 n <= 805, u16 n <= 805, u32 n <= 805, u64 n <= 805,
 *            u128 n <= 805, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static state kept between calls */
static seq_t  *runs      = NULL;      /* array of run lengths */
static size_t  runs_len  = 0;         /* number of runs stored */
static size_t  runs_cap  = 0;         /* capacity of the array    */
static uint64_t cur_num   = 0;        /* next integer to process  */
static bool     last_was_zero = false;/* flag for previous digit   */
static seq_t    cur_run   = 0;        /* length of the current run */

/* Grow the runs array to accommodate at least 'needed' elements */
static void grow_runs(size_t needed)
{
    if (needed <= runs_cap)
        return;

    size_t new_cap = runs_cap ? runs_cap * 2 : 64;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *new_ptr = (seq_t *)realloc(runs, new_cap * sizeof(seq_t));
    if (!new_ptr)
        exit(EXIT_FAILURE);  /* allocation failed */

    runs = new_ptr;
    runs_cap = new_cap;
}

/* Return the length of the n-th run of zeros (n >= 1) */
seq_t a(idx_t n)
{
    /* Generate runs until we have at least n of them */
    while (runs_len < (size_t)n)
    {
        uint64_t temp = cur_num;

        /* Special case: 0 has a single digit 0 */
        if (temp == 0)
        {
            if (last_was_zero)
                cur_run++;
            else
                cur_run = 1;
            last_was_zero = true;
        }
        else
        {
            /* Extract digits of temp in base 6 (least‑to‑most) */
            while (temp > 0)
            {
                uint64_t d = temp % 6;          /* current digit */
                if (d == 0)
                {
                    if (last_was_zero)
                        cur_run++;
                    else
                        cur_run = 1;
                }
                else
                {
                    if (last_was_zero)
                    {
                        grow_runs(runs_len + 1); /* ensure space */
                        runs[runs_len++] = cur_run;
                    }
                }
                last_was_zero = (d == 0);
                temp /= 6;
            }
        }

        /* Move to the next integer */
        cur_num++;
    }

    /* The n-th run is stored at index n-1 */
    return runs[n - 1];
}
