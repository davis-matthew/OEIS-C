/*
 * A035649  Number of partitions of n into parts 6k+3 and 6k+4 with at least
 *          one part of each type.
 * https://oeis.org/A035649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 138, u32 n <= 391, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 85 terms through n = 995 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A035649
    Number of partitions of n into parts 6k+3 and 6k+4
    with at least one part of each type.
    The method uses a standard coin–change dynamic programming
    that counts all partitions with the two allowed part sizes,
    then subtracts the partitions that use only one type.
*/

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    /* work with a plain C array sized n+1   */
    size_t N = (size_t)n;
    seq_t *total = mem_new(seq_t, N + 1);
    seq_t *only3  = mem_new(seq_t, N + 1);
    seq_t *only4  = mem_new(seq_t, N + 1);

    /* if allocation failed – return 0 (should not happen for n≤10000) */
    if (!total || !only3 || !only4) {
        mem_free(total); mem_free(only3); mem_free(only4);
        return 0;
    }

    /* base case: empty partition */
    total[0] = only3[0] = only4[0] = 1;

    /* partitions using parts 6k+3 (3,9,15,…) */
    for (idx_t coin = 3; coin <= n; coin += 6) {
        for (idx_t i = coin; i <= n; ++i) {
            total[i] += total[i - coin];
            only3[i]  += only3[i - coin];
        }
    }

    /* partitions using parts 6k+4 (4,10,16,…) */
    for (idx_t coin = 4; coin <= n; coin += 6) {
        for (idx_t i = coin; i <= n; ++i) {
            total[i] += total[i - coin];
            only4[i]  += only4[i - coin];
        }
    }

    seq_t result = total[n];
    result -= only3[n];
    result -= only4[n];

    mem_free(total); mem_free(only3); mem_free(only4);
    return result;
}
