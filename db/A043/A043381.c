/*
 * A043381  Numbers having one 3 in base 6.
 * https://oeis.org/A043381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9896 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: positive integers whose base‑6 representation contains exactly one digit 3 */
/* This function builds the sequence lazily and caches the results in a static array. */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

static inline int has_one_three_in_base6(seq_t x)
{
    int count = 0;
    while (x > 0)
    {
        if ((x % 6) == 3)
        {
            ++count;
            if (count > 1)
                return 0;
        }
        x /= 6;
    }
    return count == 1;
}

seq_t a(idx_t n)
{
    static seq_t *seq    = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static seq_t next_val  = 3;          /* value to test next in the scan */

    /* Grow the cache until it contains at least n terms */
    while (seq_len < n)
    {
        if (has_one_three_in_base6(next_val))
        {
            if (seq_len + 1 > seq_cap)
            {
                size_t new_cap = seq_cap ? seq_cap << 1 : 128;
                seq_t *tmp = mem_resize(seq_t, seq, new_cap);
                if (!tmp) abort(); /* oops: allocation failed */
                seq   = tmp;
                seq_cap = new_cap;
            }
            seq[seq_len++] = next_val;
        }
        ++next_val;
    }

    return seq[n - 1];
}
