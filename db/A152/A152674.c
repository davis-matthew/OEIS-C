/*
 * A152674  Number of divisors of the numbers that are not squares.
 * https://oeis.org/A152674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A000005(A000037(n)) - the number of divisors of the n-th
 *            non‑square
 * Range:     u8 n <= 56, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0 */

    /* Find the n-th non‑square.  Start scanning from 2 upwards. */
    idx_t  m      = 1;              /* current integer being examined   */
    idx_t  count  = 0;              /* how many non‑squares seen so far */

    while (1) {
        ++m;                       /* move to next integer             */
        seq_t t = floor_sqrt((seq_t)m);
        if (t * t == (seq_t)m)     /* m is a perfect square → skip     */
            continue;
        ++count;                   /* m is a non‑square                 */
        if (count == n)            /* found the n‑th non‑square         */
            break;
    }

    /* Compute the divisor count of m. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   pfCnt = 0;
    factor_u64((uint64_t)m, primes, exps, &pfCnt);

    seq_t d = 1;
    for (size_t i = 0; i < pfCnt; ++i)
        d *= (seq_t)(exps[i] + 1);

    return d;
}
