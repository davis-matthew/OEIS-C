/*
 * A164886  (Length of preperiodic part) + (length of cycle) of trajectory of
 *          n under iteration of the base-2 Kaprekar map in A164884.
 * https://oeis.org/A164886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 769, u16 n <= 1024, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   kaprekar_base2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS sequence A164886.
   For a given non‑negative integer n, it iterates the base‑2 Kaprekar map
   f(n) = (2^k – 1)·(2^(L‑k) – 1)
   where L = ⌊log₂ n⌋+1 and k = popcount(n).  The value returned is
   the number of distinct values visited starting from n until a value
   repeats (i.e., the sum of the preperiodic part and the cycle length). */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/kaprekar_base2.h"

/* Returns the required sequence value for index `n`. */
seq_t a(idx_t n)
{
    /* The trajectory is short for all n≤2^64−1; 256 slots are ample. */
    const size_t MAX_VISITED = 256;
    seq_t visited[MAX_VISITED];
    size_t count = 0;

    seq_t cur = (seq_t)n;
    for (;;) {
        /* Detect repetition by linear search. */
        size_t i;
        for (i = 0; i < count; ++i) {
            if (visited[i] == cur)
                return (seq_t)count;
        }

        /* Store the current value and advance. */
        if (count >= MAX_VISITED)   /* safety guard; should never hit */
            return (seq_t)count;
        visited[count++] = cur;
        cur = kaprekar_base2(cur);
    }
}
