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
 * Helpers:   has_diff4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_diff4.h"

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
