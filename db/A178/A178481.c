/*
 * A178481  Number of steps of the map x -> A055566(x), starting at n, before
 *          reaching the end of the cycle.
 * https://oeis.org/A178481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   digit_sum, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of steps of the map x -> digit_sum(x^5)
 *            starting from n, until a value repeats.
 *
 * Algorithm: keep a dynamic array of seen values, initially containing
 * the starting value n.  Repeatedly compute y = digit_sum(x^5).
 * If y has been seen before, return the current step count; otherwise
 * add y to the list and continue.  The step count is incremented
 * AFTER adding a new value, so that a(n)=0 for fixed points (0 and 1)
 * and matches the example values from OEIS.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* 0 and 1 are fixed points – they have zero steps. */
    if (n == 0 || n == 1) {
        return (seq_t)0;
    }

    /* dynamic array for values that have been seen */
    size_t cap   = 16;
    size_t len   = 0;
    seq_t *seen  = (seq_t *)malloc(cap * sizeof(seq_t));
    if (seen == NULL) {
        return (seq_t)0;        /* memory failure – safe default */
    }

    /* start with the initial value itself in the seen set */
    seen[len++] = (seq_t)n;
    seq_t cur = (seq_t)n;
    seq_t step = 0;

    while (1) {
        /* compute the next value: digit_sum of cur^5 */
        seq_t val  = pow_uint((uint64_t)cur, (idx_t)5);
        seq_t next = digit_sum(val);

        /* linear search to detect repetition */
        size_t i;
        for (i = 0; i < len; ++i) {
            if (seen[i] == next) {
                break;
            }
        }

        if (i < len) {            /* repetition found */
            free(seen);
            return step;
        }

        /* store the new value */
        if (len == cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(seen, new_cap * sizeof(seq_t));
            if (tmp == NULL) {
                free(seen);
                return step;     /* allocation failure – return what we have */
            }
            seen = tmp;
            cap  = new_cap;
        }
        seen[len++] = next;

        cur  = next;
        step += 1;
    }
}
