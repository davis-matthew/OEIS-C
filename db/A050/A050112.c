/*
 * A050112  a(n) = floor(a(n-1)/4) if this is positive and not yet in the
 *          sequence, otherwise a(n) = 6*a(n-1).
 * https://oeis.org/A050112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n>1, a(n) = a(n-1)/4 if this is positive and not
 *            yet in the sequence, otherwise a(n) = 6*a(n-1)
 * Range:     u8 n <= 11, u16 n <= 138, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static storage for memoization */
static seq_t *seq = NULL;          /* array of already computed terms */
static idx_t  seq_size = 0;        /* number of terms stored (a(1) at seq[0]) */
static idx_t  seq_capacity = 0;    /* allocated capacity */

/* Resize storage to at least new_cap elements */
static void seq_resize(idx_t new_cap)
{
    seq_t *new_buf = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!new_buf) abort();          /* unrecoverable allocation failure */
    seq = new_buf;
    seq_capacity = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset 1: we never call with n=0 */

    /* Compute missing terms up to n */
    if (seq_size < n) {
        if (seq_size == 0) {        /* initialisation */
            seq_resize(16);         /* start with a small capacity */
            seq[0] = (seq_t)1;
            seq_size = 1;
        }
        while (seq_size < n) {
            seq_t prev = seq[seq_size - 1];
            seq_t cand = prev / (seq_t)4;  /* floor division */
            bool used = false;

            if (cand > (seq_t)0) {
                /* linear search for cand in the stored terms */
                for (idx_t i = 0; i < seq_size; ++i) {
                    if (seq[i] == cand) {
                        used = true;
                        break;
                    }
                }
            }

            seq_t next = used || cand == (seq_t)0 ? prev * (seq_t)6 : cand;

            if (seq_size == seq_capacity) {
                /* double capacity when needed */
                seq_resize(seq_capacity * 2);
            }
            seq[seq_size++] = next;
        }
    }
    return seq[n - 1];               /* convert to 0‑based index */
}
