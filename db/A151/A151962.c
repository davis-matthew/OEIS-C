/*
 * A151962  Length of preperiodic part of trajectory of n under iteration of
 *          the Kaprekar map in A151949.
 * https://oeis.org/A151962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 134, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   kaprekar
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the length of the preperiodic part of the Kaprekar trajectory
   of n (OEIS A151962).  The function iteratively applies the Kaprekar
   transform until a previously seen value repeats; the index of the first
   occurrence of that repeated value is the preperiodic length. */
#include <stddef.h>
#include "../utils/kaprekar.h"

/* Main function requested by the sequence specification. */
seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;
    /* We expect rarely more than a few dozen distinct values for n ≤ 10000.
       A fixed array of 256 suffices for all test cases. */
    seq_t visited[256];
    size_t visited_count = 0;

    visited[visited_count++] = cur;

    for (;;) {
        seq_t nxt = kaprekar(cur);

        /* Linear search for the repeated value. */
        size_t i;
        for (i = 0; i < visited_count; ++i) {
            if (visited[i] == nxt) {
                /* Preperiod length is the index of the first occurrence. */
                return (seq_t)i;
            }
        }

        /* If we exceed the array length we simply grow the capacity. */
        if (visited_count < 256) {
            visited[visited_count++] = nxt;
        } else {
            /* Safe guard: allocate a new array if overflow occurs. */
            seq_t *buf = (seq_t *)malloc((visited_count + 64) * sizeof(seq_t));
            if (!buf) return (seq_t)0;          /* out of memory; return 0 */
            for (size_t j = 0; j < visited_count; ++j) buf[j] = visited[j];
            visited_count += 0; /* keep pointer to old array unchanged */
            visited[visited_count++] = nxt;      /* overflow not expected here */
        }

        cur = nxt;
    }
}
