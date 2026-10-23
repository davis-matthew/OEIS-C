/*
 * A037804  Number of i such that d(i) < d(i-1), where Sum_{i=0..m} d(i)*6^i
 *          is the base-6 representation of n.
 * https://oeis.org/A037804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of indices i ≥ 1 such that d(i) < d(i‑1), where n
 *            = Σ_{i=0}^m d(i)·6^i is the base‑6 representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base‑6 digit extraction and comparison. */
    seq_t count = 0;
    unsigned int prev = n % 6;  /* d(0) */
    n /= 6;
    while (n > 0) {
        unsigned int cur = n % 6;   /* d(i) for current i */
        if (cur < prev)
            count++;
        prev = cur;
        n /= 6;
    }
    return count;
}
