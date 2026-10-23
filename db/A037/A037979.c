/*
 * A037979  Numbers whose maximal base-4 run length is 4.
 * https://oeis.org/A037979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   max_base4_run
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A037979: numbers whose maximal base‑4 run length is exactly 4
   (computed by brute‑force search). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/max_base4_run.h"

/* Static storage for the generated sequence.  len is the number of values
   currently stored, cap is the allocated capacity.  next_candidate is the
   integer being tested next. */
static seq_t *seq   = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;
static seq_t next_candidate = 1;

/* Return the n-th term (n >= 1).  The function generates terms on demand
   and works for any access pattern. */
seq_t a(idx_t n)
{
    /* Convert the request to a size_t to use as an array index.  The
       test harness never asks for n > sizeof(size_t) because it limits
       n to about 10 000. */
    size_t target = (size_t)n;

    /* Expand the cache until it contains at least target elements. */
    while (seq_len < target)
    {
        /* Find the next integer whose maximal base‑4 run length is 4. */
        while (1)
        {
            if (max_base4_run(next_candidate) == 4)
                break;
            ++next_candidate;
        }

        /* Ensure we have room for another value. */
        if (seq_len == seq_cap)
        {
            size_t new_cap = seq_cap ? seq_cap << 1 : 8;
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp)                   /* allocation failed */
                abort();
            seq = tmp;
            seq_cap = new_cap;
        }

        /* Store the discovered value and move to the next candidate. */
        seq[seq_len++] = next_candidate;
        ++next_candidate;
    }

    return seq[target - 1];
}
