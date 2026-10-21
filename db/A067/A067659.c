/*
 * A067659  Number of partitions of n into distinct parts such that number of
 *          parts is odd.
 * https://oeis.org/A067659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of partitions of n into distinct parts with an odd
 *            number of parts. Computation method: dynamic programming that
 *            keeps track of even‐ and odd‐parity counts while adding each
 *            possible part. Complexity: O(n²) time, O(n) space (n≤10000
 *            satisfies the time limit)
 * Range:     u8 n <= 33, u16 n <= 85, u32 n <= 250, u64 n <= 813,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* a(0) = 0 by definition */

    /* Allocate arrays that hold even‑ and odd‑parity partition counts. */
    size_t len = (size_t)n + 1;
    seq_t *even = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *odd  = (seq_t *)calloc(len, sizeof(seq_t));
    if (!even || !odd)
    {
        free(even);
        free(odd);
        return (seq_t)0;          /* allocation failure – return 0 */
    }

    even[0] = (seq_t)1;          /* empty sum has 0 parts (even) */

    /* Add each possible part once (distinct parts). */
    for (idx_t part = 1; part <= n; ++part)
    {
        for (idx_t sum = n; sum >= part; --sum)
        {
            seq_t e = even[sum - part];
            seq_t o = odd[sum - part];
            even[sum] += o;      /* adding an odd‑part pair toggles parity */
            odd[sum]  += e;
        }
    }

    seq_t result = odd[n];
    free(even);
    free(odd);
    return result;
}
