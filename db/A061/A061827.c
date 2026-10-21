/*
 * A061827  Number of partitions of n into parts which are the digits of n.
 * https://oeis.org/A061827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = number of unordered partitions of n into parts that are
 *            the non‑zero decimal digits of n. For each non‑zero digit d of
 *            n, we may use an arbitrary number of parts of size d; the order
 *            of parts does not matter. The number of such partitions is
 *            computed by the classic coin‑change DP with coin...
 * Range:     u8 n <= 122, u16 n <= 1022, u32 n <= 9975, u64 n <= 9975,
 *            u128 n <= 9975, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* handle trivial case quickly */
    if (n == 0) return 0;

    /* mark which non‑zero digits appear in n */
    bool present[10] = {false};
    idx_t tmp = n;
    while (tmp > 0) {
        unsigned d = (unsigned)(tmp % 10);
        if (d > 0) present[d] = true;
        tmp /= 10;
    }

    /* DP array: ways[i] = number of partitions of i */
    seq_t *ways = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!ways) return 0;          /* allocation failed, return 0 as a fallback */

    ways[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) ways[i] = (seq_t)0;

    /* unlimited coins of each present digit */
    for (int d = 1; d <= 9; ++d) {
        if (!present[d]) continue;
        for (idx_t i = d; i <= n; ++i) {
            ways[i] += ways[i - d];
        }
    }

    seq_t result = ways[n];
    free(ways);
    return result;
}
