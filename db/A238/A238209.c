/*
 * A238209  The total number of 2's in all partitions of n into an odd number
 *          of distinct parts.
 * https://oeis.org/A238209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = total number of 2's in all partitions of n into an odd
 *            number of distinct parts. Computed by dynamic programming over
 *            distinct parts, keeping counts for even/odd number of parts and
 *            the corresponding total number of 2's
 * Range:     u8 n <= 40, u16 n <= 94, u32 n <= 264, u64 n <= 837,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 77 terms through n = 996 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Allocate temporary arrays */
    size_t size = (size_t)(n + 1);
    seq_t *cnt_even = mem_new(seq_t, size);
    seq_t *cnt_odd  = mem_new(seq_t, size);
    seq_t *twos_even = mem_new(seq_t, size);
    seq_t *twos_odd  = mem_new(seq_t, size);

    if (!cnt_even || !cnt_odd || !twos_even || !twos_odd)
        return (seq_t)0; /* allocation failure, return 0 */

    /* Base case: sum 0 with 0 parts (even) */
    cnt_even[0] = 1;

    /* DP over distinct parts */
    for (idx_t part = 1; part <= n; ++part)
    {
        for (idx_t sum = n; sum >= part; --sum)
        {
            idx_t rem = sum - part;

            seq_t ce = cnt_even[rem];
            seq_t co = cnt_odd[rem];
            seq_t te = twos_even[rem];
            seq_t to = twos_odd[rem];

            /* Adding part flips parity */
            cnt_odd[sum] += ce;
            twos_odd[sum] += te + ((part == (idx_t)2) ? ce : 0);

            cnt_even[sum] += co;
            twos_even[sum] += to + ((part == (idx_t)2) ? co : 0);
        }
    }

    seq_t result = twos_odd[n];

    mem_free(cnt_even);
    mem_free(cnt_odd);
    mem_free(twos_even);
    mem_free(twos_odd);

    return result;
}
