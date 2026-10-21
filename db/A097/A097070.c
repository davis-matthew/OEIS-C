/*
 * A097070  Consider all compositions (ordered partitions) of n into n parts,
 *          allowing zeros. E.g., for n = 3 we get 300, 030, 003, 210, 120,
 *          201, 102, 021, 012, 111. Then a(n) is the total number of 1's.
 * https://oeis.org/A097070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * binomial(2*n-3, n-1) for n >= 2; a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* out of defined range, return 0 */
    if (n == 1)
        return 1;              /* a(1) = 1 by definition */

    /* Compute n * C(2n-3, n-1) */
    idx_t num = 2 * n - 3;     /* 2n-3 */
    idx_t k   = n - 1;         /* n-1 */

    seq_t bin = binomial(num, k);   /* binomial(num, k) uses the provided lib */
    return (seq_t)n * bin;
}
