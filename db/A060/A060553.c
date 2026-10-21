/*
 * A060553  a(n) is the number of distinct (modulo geometric D3-operations)
 *          patterns which can be formed by an equilateral triangular
 *          arrangement of closely packed black and white cells satisfying the
 *          local matching rule of Pascal's triangle modulo 2, where n is the
 *          number of cells in each edge of the arrang...
 * https://oeis.org/A060553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^(n-1) + 2^(floor(n/3) + (n mod 3) mod 2)) / 3 +
 *            2^floor((n-1)/2) (n ≥ 1)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 129,
 *            big any n
 * Helpers:   pow2_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset starts at 1 */

    /* exponents used in the formula */
    idx_t exp1 = n - 1;
    idx_t exp2 = (n / 3) + ((n % 3) % 2);
    idx_t exp3 = (n - 1) / 2;

    /* If both exp1 and exp2 fit in 127 bits, use a 128‑bit intermediate
       to avoid overflow.  This covers all n where the final result
       still fits in a 64‑bit integer (up to n = 66).            */
#if defined(__SIZEOF_INT128__)
    if (exp1 <= 127 && exp2 <= 127)
    {
        unsigned __int128 term1 = ((unsigned __int128)1) << exp1;
        unsigned __int128 term2 = ((unsigned __int128)1) << exp2;
        unsigned __int128 sum   = term1 + term2;
        unsigned __int128 q     = sum / 3;
        unsigned __int128 term3 = ((unsigned __int128)1) << exp3;
        return (seq_t)(q + term3);
    }
#endif

    /* General case: use big‑integer arithmetic via pow2_big. */
    seq_t term1 = pow2_big(exp1);
    seq_t term2 = pow2_big(exp2);
    seq_t sum   = term1 + term2;
    seq_t q     = sum / 3;
    seq_t term3 = pow2_big(exp3);
    return q + term3;
}
