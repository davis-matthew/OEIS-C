/*
 * A122530  Decreasing runs. The size of each run is given by the sequence
 *          itself. First run starts with "1", second with "2", third with
 *          "3", fourth with "4", etc.
 * https://oeis.org/A122530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates A122530: Decreasing runs.  
   Run k starts with integer k and has length a(k).  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < (idx_t)1) {
        return (seq_t)0; /* undefined for n < 1 */
    }

    /* Static storage for the sequence built on demand */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;   /* number of terms currently stored   */
    static size_t seq_cap = 0;   /* allocated capacity                 */

    /* Run‑generation state */
    static idx_t next_run_number = 0;   /* next run number to start (k) */
    static seq_t cur_val_in_run = 0;    /* current value in the active run */
    static size_t rem_in_run = 0;       /* remaining elements in the current run after the first */

    /* Extend the sequence until it contains at least n terms */
    while (seq_len < (size_t)n) {
        if (rem_in_run == 0) {                  /* start a new run */
            next_run_number++;                 /* run number k */

            /* Ensure there is space for the first element of this run */
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                seq = tmp;
                seq_cap = new_cap;
            }

            /* First element of the run is k itself */
            seq[seq_len++] = (seq_t)next_run_number;
            cur_val_in_run = (seq_t)next_run_number;

            if (next_run_number == 1) {          /* first run: length 1 */
                rem_in_run = 0;
            } else {
                /* Length of the run is a(k) = seq[k-1] (already stored) */
                size_t L = (size_t)seq[next_run_number - 1];
                rem_in_run = (L > 0) ? L - 1 : 0;
            }
        } else {                                /* continue current run */
            cur_val_in_run = cur_val_in_run - 1;

            /* Ensure space for the next element of this run */
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                seq = tmp;
                seq_cap = new_cap;
            }

            seq[seq_len++] = cur_val_in_run;
            rem_in_run--;
        }
    }

    return seq[n - 1];
}
