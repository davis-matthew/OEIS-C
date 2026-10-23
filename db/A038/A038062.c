/*
 * A038062  Number of labeled trees with 3-colored nodes.
 * https://oeis.org/A038062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n-2) * 3^n for n ≥ 1, a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses helper seq_pow(seq_t base, idx_t exp) from ../utils/seq_pow.h */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;       /* Only the empty tree */
    if (n == (idx_t)1)
        return (seq_t)3;       /* One node, 3 possible colors */
    /* For n ≥ 2: number of labeled trees is n^(n-2) and each node can be
       colored independently with 3 colors. */
    seq_t t1 = seq_pow((seq_t)n, n - 2);   /* n^(n-2) */
    seq_t t2 = seq_pow((seq_t)3, n);       /* 3^n      */
    return t1 * t2;                       /* product gives a(n) */
}
