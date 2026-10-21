/*
 * A216022  Largest number m such that the Collatz trajectory starting at n
 *          contains all numbers not greater than m.
 * https://oeis.org/A216022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A216022: a(n) = largest m such that the Collatz trajectory from n contains all numbers ≤ m (m ≤ 6). */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t curr = (seq_t)n;
    bool seen[7] = { false }; /* indices 0..6, we only use 1..6 */

    while (1) {
        if (curr <= 6)
            seen[(int)curr] = true;
        if (curr == (seq_t)1)
            break;
        if (curr & (seq_t)1)            /* odd */
            curr = (seq_t)3 * curr + (seq_t)1;
        else                          /* even */
            curr >>= 1;
    }

    seq_t max_prefix = 1;
    for (int i = 1; i <= 6; ++i) {
        if (!seen[i])
            break;
        max_prefix = (seq_t)i;
    }
    return max_prefix;
}
