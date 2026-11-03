/*
 * A239263  Number of partitions of n having (sum of odd parts) >= (sum of
 *          even parts).
 * https://oeis.org/A239263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = \sum_{O=\lceil n/2\rceil}^{n} p_{odd}(O)\,p_{even}(n-O)
 * Range:     u8 n <= 18, u16 n <= 46, u32 n <= 133, u64 n <= 427,
 *            u128 n <= 1478, big any n
 * Validated: standard, pass, 65 terms through n = 9724 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          where p_{odd}(k) = #partitions of k into odd parts,   */
/*                p_{even}(k) = #partitions of k into even parts. */
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* allocate space for the two partition arrays */
    seq_t *p_odd  = mem_new(seq_t, (size_t)(n + 1));
    seq_t *p_even = mem_new(seq_t, (size_t)(n + 1));
    if (!p_odd || !p_even) {
        mem_free(p_odd);
        mem_free(p_even);
        return 0;
    }

    /* base case: one way to partition 0 */
    p_odd[0]  = 1;
    p_even[0] = 1;

    /* Partitions into odd parts only */
    for (idx_t part = 1; part <= n; part += 2) {
        for (idx_t sum = part; sum <= n; ++sum)
            p_odd[sum] += p_odd[sum - part];
    }

    /* Partitions into even parts only */
    for (idx_t part = 2; part <= n; part += 2) {
        for (idx_t sum = part; sum <= n; ++sum)
            p_even[sum] += p_even[sum - part];
    }

    /* Count partitions with odd-sum >= even-sum */
    seq_t result = 0;
    idx_t start = (n + 1) / 2;              /* ceil(n/2) */
    for (idx_t O = start; O <= n; ++O) {
        seq_t oddCnt  = p_odd[O];
        seq_t evenCnt = p_even[n - O];
        result += oddCnt * evenCnt;
    }

    mem_free(p_odd);
    mem_free(p_even);
    return result;
}
