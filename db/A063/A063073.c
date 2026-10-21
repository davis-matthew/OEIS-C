/*
 * A063073  Square of determinant of character table of the dihedral group
 *          with 2n elements.
 * https://oeis.org/A063073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * n^((n+1)/2) if n odd, a(n) = 64 * n^((n+2)/2) if n
 *            even
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 23, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1U)
            result = result * base;
        base = base * base;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) /* undefined for n=0, but return 0 for safety */
        return (seq_t)0;

    if (n & 1U) /* odd n */
    {
        idx_t exp = (n + 1) >> 1U;          /* (n+1)/2 */
        seq_t p   = pow_seq_t((seq_t)n, exp);
        return ((seq_t)4) * p;               /* 4 * n^((n+1)/2) */
    }
    else /* even n */
    {
        idx_t exp = (n + 2) >> 1U;          /* (n+2)/2 */
        seq_t p   = pow_seq_t((seq_t)n, exp);
        return ((seq_t)64) * p;              /* 64 * n^((n+2)/2) */
    }
}
