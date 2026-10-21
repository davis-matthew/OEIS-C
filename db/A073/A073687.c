/*
 * A073687  Group the composite numbers so that the sum of each group is a
 *          prime; sequence gives first term in each group.
 * https://oeis.org/A073687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   by grouping consecutive composite numbers so that the sum * of
 *            each group is prime; a(n) is the first term in the n‑th group
 * Range:     u8 n <= 4, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeTrial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* undefined for n < 1 */

    /* Persistent state across calls */
    static seq_t *heads = NULL;     /* array of group first terms          */
    static size_t cap = 0;          /* allocated capacity of heads         */
    static idx_t count = 0;         /* number of groups computed so far    */
    static idx_t current = 4;       /* next integer to consider            */

    /* Generate groups until we have at least n groups */
    while (count < n) {
        /* Find the first composite for this group */
        while (current < 4 || isPrimeTrial((seq_t)current))
            ++current;
        seq_t start = (seq_t)current;

        /* Accumulate composites until the running sum is prime */
        seq_t sum = 0;
        while (true) {
            sum += (seq_t)current;
            if (isPrimeTrial(sum))
                break;           /* group ends */

            ++current;
            /* Skip non‑composites for the next iteration */
            while (current < 4 || isPrimeTrial((seq_t)current))
                ++current;
        }

        /* Store the group head */
        if (count == cap) {
            size_t new_cap = cap ? cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(heads, new_cap * sizeof(seq_t));
            if (!tmp)
                exit(1);          /* out of memory */
            heads = tmp;
            cap = new_cap;
        }
        heads[count++] = start;

        /* Advance current past the last composite in this group */
        ++current;
    }

    return heads[n - 1];
}
