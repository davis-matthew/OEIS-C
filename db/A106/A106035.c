/*
 * A106035  The "Octanacci" sequence: Trajectory of 1 under the morphism
 *          1->{1,2,1}, 2->{1}.
 * https://oeis.org/A106035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n) is the nth element of the fixed point of the
 *            morphism 1 → {1,2,1}, 2 → {1}
 * Range:     u8 n <= 1392, u16 n <= 1392, u32 n <= 1392, u64 n <= 1392,
 *            u128 n <= 1392, big any n
 * Validated: standard, pass, 121 terms through n = 1193 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *seq = NULL;   /* dynamic array holding generated terms */
static idx_t seq_size = 0;  /* current number of stored terms */

seq_t a(idx_t n)
{
    /* ensure at least the first term exists */
    if (seq == NULL) {
        seq = mem_new(seq_t, 1);
        seq[0] = (seq_t)1;
        seq_size = 1;
    }

    /* expand the word until it contains at least (n+1) terms */
    while (n >= seq_size) {
        /* count how many '1's are present in the current word */
        idx_t ones = 0;
        for (idx_t i = 0; i < seq_size; ++i)
            if (seq[i] == (seq_t)1)
                ++ones;

        /* each '1' expands to 3 symbols, each '2' stays as 1 symbol,
           so new length = current length + 2 * (#ones)           */
        idx_t new_size = seq_size + 2 * ones;

        /* allocate space for the expanded word */
        seq_t *new_seq = mem_new(seq_t, new_size);
        idx_t pos = 0;
        for (idx_t i = 0; i < seq_size; ++i) {
            if (seq[i] == (seq_t)1) {
                new_seq[pos++] = (seq_t)1;
                new_seq[pos++] = (seq_t)2;
                new_seq[pos++] = (seq_t)1;
            } else { /* seq[i] == 2 */
                new_seq[pos++] = (seq_t)1;
            }
        }

        /* discard old word and keep the new one */
        mem_free(seq);
        seq     = new_seq;
        seq_size = new_size;
    }

    return seq[n];
}
