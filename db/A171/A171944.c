/*
 * A171944  N-positions for game of misere version of Mark.
 * https://oeis.org/A171944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th non‑negative integer i that is a winning (N)
 *            position in the misère Mark game. The game moves are: (1)
 *            subtract 1 stone → new size i‑1 (2) subtract ceil(i/2) stones →
 *            new size floor(i/2) A position is winning iff at least one legal
 *            move takes it to a losing position. The sequence...
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static tables holding the N‑positions (seq) and the status of each size
       (status[i] == true  ⇔  i is a winning position). */
    static seq_t *seq       = NULL;
    static size_t seq_cap   = 0, seq_len   = 0;
    static bool *status     = NULL;
    static size_t status_cap = 0;
    static idx_t processed   = 0;            /* largest i for which status[i] is known */

    /* initialise on first call */
    if (status == NULL) {
        status_cap = 1;
        status = (bool *)malloc(status_cap * sizeof(bool));
        status[0] = true;                    /* position 0 is winning in misère play */
    }
    if (seq == NULL) {
        seq_cap    = 1;
        seq        = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0]     = (seq_t)0;              /* a(1) = 0 */
        seq_len    = 1;
    }

    /* generate more positions until we have at least n of them */
    while (seq_len < n) {
        /* double the range for the status table (at least up to 1 if not yet) */
        idx_t new_limit = (processed == 0) ? 1 : processed * 2;
        if (new_limit + 1 > status_cap) {
            status_cap = new_limit + 1;
            status = (bool *)realloc(status, status_cap * sizeof(bool));
        }

        /* compute statuses for all i in (processed, new_limit] */
        for (idx_t i = processed + 1; i <= new_limit; ++i) {
            bool prev_N = status[i - 1];     /* status of i-1   */
            bool half_N = status[i / 2];     /* status of floor(i/2) */
            status[i] = (!prev_N) || (!half_N);   /* N if any move to P */

            if (status[i]) {                 /* i is an N‑position */
                if (seq_len + 1 > seq_cap) {
                    seq_cap = seq_cap * 2;
                    seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                }
                seq[seq_len++] = (seq_t)i;
            }
        }
        processed = new_limit;
    }

    return seq[n - 1];    /* a(1) is stored at seq[0] */
}
