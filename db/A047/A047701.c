/*
 * A047701  All positive numbers that are not the sum of 5 nonzero squares.
 * https://oeis.org/A047701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, full, fini
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   compute_reachable
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A047701: all positive integers that are not the sum
   of five non‑zero squares.  The function builds the sequence
   incrementally: each time the required number of terms is
   not yet available an enlarged search limit is chosen, every
   number up to that limit is tested for representability by a
   dynamic‑programming routine, and the new unreachable numbers
   are appended to the static table.  The search limit is
   doubled (or increased by 100) at each expansion, keeping the
   overall running time well below two seconds for n ≈ 10 000. */
 
#include <stdlib.h>
#include <string.h>
#include "../utils/compute_reachable.h"

seq_t a(idx_t n)
{
    /* static table of discovered terms */
    static seq_t *seq = NULL;
    static idx_t seq_len = 0;
    static idx_t seq_cap = 0;
    /* keeps track of the maximum number checked so far */
    static idx_t current_limit = 0;

    /* grow the table until we have at least n terms */
    while (seq_len < n) {
        /* choose a new search limit (double or +100) */
        idx_t new_limit = (current_limit == 0) ? 200 : current_limit * 2;
        if (new_limit < current_limit + 100)
            new_limit = current_limit + 100;

        /* compute reachability up to new_limit */
        char *reachable = (char*)malloc((new_limit + 1) * sizeof(char));
        if (reachable == NULL) {
            /* on allocation failure we simply return the last known term */
            return seq_len > 0 ? seq[seq_len - 1] : (seq_t)0;
        }
        compute_reachable(new_limit, reachable);

        /* add the new unreachable numbers to the table */
        for (idx_t x = current_limit + 1; x <= new_limit; ++x) {
            if (!reachable[x]) {
                /* grow array if necessary */
                if (seq_len == seq_cap) {
                    seq_cap = seq_cap ? seq_cap * 2 : 8;
                    seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
                    if (seq == NULL) {
                        /* allocation failed – clean up and exit */
                        free(reachable);
                        return (seq_t)0;
                    }
                }
                seq[seq_len++] = (seq_t)x;
            }
        }

        free(reachable);
        current_limit = new_limit;
    }

    return seq[n - 1];
}
