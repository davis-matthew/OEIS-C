/*
 * A127141  Number of iterations needed before a previous value is reached in
 *          the Collatz variation: x_{i+1} = 3x_i+(2n+1) if x_i is odd or
 *          x_i/2 if x_i is even, with x_0 = 1.
 * https://oeis.org/A127141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A127141
   a(n) = number of iterations needed before a previous value is reached
   in the Collatz variant
          x_{i+1} = 3*x_i + (2*n + 1)  if x_i is odd
                = x_i / 2                if x_i is even
   starting from x_0 = 1.
   The function returns a(n) for the given index n (0‑based). */

#include <stddef.h>
#include <stdbool.h>

#if defined(__cplusplus)
/* For the big‑integer build the utilities below correctly construct
   and destroy objects of type seq_t.  */
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"
# define SEQ_ALLOC(SZ)    alloc_seq_array((size_t)(SZ))
# define SEQ_FREE(P)      free_seq_array((seq_t*)(P))
#else
/* Plain C build – seq_t is an integral type.  */
# include <stdlib.h>
# define SEQ_ALLOC(SZ)    ((seq_t*)malloc(sizeof(seq_t)*(SZ)))
# define SEQ_FREE(P)      free((void*)(P))
#endif

seq_t a(idx_t n)
{
    /* The increment added on odd steps. */
    seq_t inc = (seq_t)n * 2 + 1;

    /* Current value of the sequence. */
    seq_t x = 1;

    /* Dynamic array of seen values. */
    seq_t *seen = NULL;
    size_t seen_cap = 0;
    size_t seen_len = 0;

    /* Helper to grow the seen array. */
    if (seen_cap == 0) {
        seen_cap = 8;
        seen = SEQ_ALLOC(seen_cap);
    }

    while (1) {
        /* Check whether x has already been seen. */
        bool repeat = false;
        for (size_t i = 0; i < seen_len; ++i) {
            if (seen[i] == x) { repeat = true; break; }
        }
        if (repeat) {
            SEQ_FREE(seen);
            return (seq_t)seen_len;
        }

        /* Record the current value. */
        if (seen_len == seen_cap) {
            size_t new_cap = seen_cap * 2;
            seq_t *tmp = SEQ_ALLOC(new_cap);
            for (size_t i = 0; i < seen_len; ++i) tmp[i] = seen[i];
            SEQ_FREE(seen);
            seen = tmp;
            seen_cap = new_cap;
        }
        seen[seen_len++] = x;

        /* Compute next value. */
        if ((x & 1) == 0) {
            x = x / 2;
        } else {
            x = x * 3 + inc;
        }
    }
}
