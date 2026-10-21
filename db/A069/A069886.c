/*
 * A069886  Smallest multiple of n starting with 1 with descending digit
 *          string (a left substring of 10987654321098...), 0 if no such
 *          number exists.
 * https://oeis.org/A069886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 10, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   residue
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest multiple of n that matches a prefix of the infinite
   descending‑digit string 10987654321098….
   Returned value is the minimal such multiple, or 0 if none exists.
   Implementation: build the prefix digit by digit,
   tracking the remainder modulo n and a cycle index of the digit
   sequence.  If a state (remainder, cycle position) repeats,
   the process loops forever and no multiple exists.
*/
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/residue.h"

/* main sequence function */
seq_t a(idx_t n)
{
    /* n ≥ 1 by specification */
    /* 12 possible cycle positions: 2 unique first digits + 10‑digit cycle */
    const size_t cycle_states = 12;
    size_t total = (size_t)n * cycle_states;
    bool *visited = (bool *)calloc(total, sizeof(bool));
    if (visited == NULL)
        return (seq_t)0;              /* out of memory → no solution */

    idx_t rem = 0;        /* current remainder modulo n */
    seq_t val = 0;        /* current prefix value */
    idx_t i = 0;          /* digit index */

    while (1) {
        unsigned d;
        if (i == 0)
            d = 1;
        else if (i == 1)
            d = 0;
        else
            d = 9 - ((i - 2) % 10);

        /* update remainder and value */
        rem = residue(rem, n, d);

        /* for arbitrary‑precision seq_t the operators work directly */
        val = val * 10 + d;

        /* compute cycle index for state tracking */
        size_t cycle_index = (i < 2) ? i : 2 + ((unsigned)(i - 2) % 10);
        size_t state_index = (size_t)rem * cycle_states + cycle_index;

        if (visited[state_index]) {
            /* repeated state ⇒ loop, no multiple exists */
            free(visited);
            return (seq_t)0;
        }
        visited[state_index] = true;

        if (rem == 0) {
            free(visited);
            return val;
        }

        i++;
        /* safety: if the number of digits grows too large, we could
           abort, but the visited set guarantees termination.    */
    }
}
