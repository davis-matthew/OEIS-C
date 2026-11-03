/*
 * A031460  Numbers whose base-3 representation has 4 fewer 0's than 2's.
 * https://oeis.org/A031460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th integer whose base‑3 representation contains four
 *            more 2’s than 0’s (i.e., #2 – #0 = 4). The function finds this
 *            term by brute‑force scanning of natural numbers
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Test whether x has #2 − #0 == 4 in its base‑3 expansion. */
static inline bool has_diff4(seq_t x)
{
    int diff = 0;
    while (x > 0)
    {
        seq_t r = x % 3;
        if (r == 0)
            diff--;
        else if (r == 2)
            diff++;
        x /= 3;
    }
    return diff == 4;
}

/* Return the n-th term of sequence A031460. */
seq_t a(idx_t n)
{
    idx_t  found   = 0;
    seq_t  number  = 1;

    while (found < n)
    {
        if (has_diff4(number))
            ++found;
        ++number;
    }
    /* number is one greater than the desired term. */
    return number - 1;
}
