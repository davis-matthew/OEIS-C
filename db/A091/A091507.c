/*
 * A091507  Product of the anti-divisors of n.
 * https://oeis.org/A091507
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of the distinct quotients s/d, where s ∈ {2n−1,
 *            2n, 2n+1} and d runs over all odd divisors >1 of s
 * Range:     u8 n <= 12, u16 n <= 31, u32 n <= 93, u64 n <= 246,
 *            u128 n <= 1000, big any n
 * Helpers:   add_unique, process_s
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the definition of A091507. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/add_unique.h"
#include "../utils/process_s.h"

/* main sequence function */
seq_t a(idx_t n)
{
    /* offset behaviour: a(3) is first term; below offset undefined */
    if (n < 3)
        return (seq_t)1;

    seq_t prod = (seq_t)1;
    seq_t uniq[1024];
    size_t uniq_len = 0;

    /* process s = 2n-1, 2n, 2n+1 */
    for (int off = -1; off <= 1; ++off) {
        idx_t s = 2 * n + (idx_t)off;
        process_s(s, uniq, &uniq_len);
    }

    for (size_t i = 0; i < uniq_len; ++i)
        prod *= uniq[i];

    return prod;
}
