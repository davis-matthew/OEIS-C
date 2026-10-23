/*
 * A036550  a(n) = T(0,n) + T(1,n-1) + ... + T(n,0), array T given by A048471.
 * https://oeis.org/A036550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^(n+1) - 2^(n+2) + 2*n + 3) / 2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 79,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

/* external linkage, per specification */
seq_t a(idx_t n)
{
#ifdef __SIZEOF_INT128__
    /* When seq_t is 8‑byte or smaller, use 128‑bit intermediates
       to avoid overflow of 3^(n+1) and 2^(n+2) for the values that
       fit into seq_t. */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        __uint128_t pow3 = 1;
        __uint128_t pow2 = 1;

        for (idx_t i = 0; i < n + 1; ++i)
            pow3 *= 3;
        for (idx_t i = 0; i < n + 2; ++i)
            pow2 *= 2;

        __uint128_t num = pow3 - pow2 + (__uint128_t)2 * n + 3;
        return (seq_t)(num / 2);
    }
#endif
    /* Fallback for arbitrary‑precision seq_t types. */
    seq_t pow3 = big_pow((seq_t)3, (seq_t)(n + 1));
    seq_t pow2 = big_pow((seq_t)2, (seq_t)(n + 2));

    seq_t num = pow3;
    num -= pow2;
    seq_t tmp = (seq_t)2 * n;
    num += tmp;
    num += 3;
    return num / 2;
}
