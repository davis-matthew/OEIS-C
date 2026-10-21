/*
 * A090326  Number of rules of a context-free grammar in Chomsky normal form
 *          that generates all permutations of n symbols.
 * https://oeis.org/A090326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - 2^(n+1) + n + 1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   pow_seq_t_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq_t_local.h"
/* a(1) = 1, a(2) = 4, a(3) = 15, ... */

seq_t a(idx_t n)
{
    if (n == 0)               /* behaviour for indices below offset is unspecified */
        return (seq_t)0;

    /* compute 3^n */
    seq_t pow3 = pow_seq_t_local((seq_t)3, n);

    /* compute 2^(n+1) */
    idx_t exp2 = n + 1;
    seq_t pow2 = pow_seq_t_local((seq_t)2, exp2);

    seq_t sn = (seq_t)n;
    return pow3 - pow2 + sn + 1;
}
