/*
 * A019510  a(n) = gcd( binomial(n+3, n) + binomial(n+4, n+1), binomial(n+5,
 *          n+2) ).
 * https://oeis.org/A019510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd( binomial(n+3, n) + binomial(n+4, n+1), binomial(n+5,
 *            n+2) )
 * Range:     u8 n <= 6, u16 n <= 343, u32 n <= 2414, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 66 terms through n = 4891 (2026-11-01)
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset starts at 1, this case is unused */

    /* binomial(n+3, n) == binomial(n+3, 3)   */
    /* binomial(n+4, n+1) == binomial(n+4, 3) */
    /* binomial(n+5, n+2) == binomial(n+5, 3) */

    const seq_t c1 = binomial(n + 3, 3);
    const seq_t c2 = binomial(n + 4, 3);
    const seq_t c3 = binomial(n + 5, 3);

    const seq_t sum = c1 + c2;
    return gcd(sum, c3);
}
