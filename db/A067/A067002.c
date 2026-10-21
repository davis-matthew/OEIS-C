/*
 * A067002  Numerator of Sum_{k=0..n} 2^(k-2*n) * binomial(2*n-2*k,n-k) *
 *          binomial(n+k,n).
 * https://oeis.org/A067002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Sum_{k=0..n} 2^(k‑2n) * binomial(2n‑2k,n‑k)
 *            * binomial(n+k,n). The sequence satisfies the recurrence 4 n
 *            (n‑1) a(n) = 8 (2n‑1)(n‑1) a(n‑1) – (16(n‑1)²‑1) a(n‑2),
 *            together with the initial values a(0)=1, a(1)=3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   gcd_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd_uint128.h"

/* seq_t and idx_t are provided by the build environment. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* The recurrence only needs the two previous terms. */
    seq_t num_nm2 = (seq_t)1;   /* a(0) */
    int    exp_nm2 = 0;         /* exponent of 2 in denominator of fr(0) */
    seq_t num_nm1 = (seq_t)3;   /* a(1) */
    int    exp_nm1 = 1;         /* exponent of 2 in denominator of fr(1) */

    for (idx_t i = 2; i <= n; ++i) {
        /* Align the denominators of the two previous terms. */
        int e1 = exp_nm1;
        int e2 = exp_nm2;
        int expR = (e1 > e2) ? e1 : e2;
        int shift1 = expR - e1;
        int shift2 = expR - e2;

        /* Compute the two terms of the right‐hand side. */
        unsigned __int128 coef1 = (unsigned __int128)8
                               * (unsigned __int128)(2 * i - 1)
                               * (unsigned __int128)(i - 1);
        unsigned __int128 val1 = coef1 * (unsigned __int128)num_nm1;
        if (shift1 > 0) val1 <<= shift1;

        unsigned __int128 coef2 = (unsigned __int128)16
                               * (unsigned __int128)(i - 1)
                               * (unsigned __int128)(i - 1)
                               - (unsigned __int128)1;
        unsigned __int128 val2 = coef2 * (unsigned __int128)num_nm2;
        if (shift2 > 0) val2 <<= shift2;

        unsigned __int128 rhs_num = val1 - val2;   /* always positive */
        int rhs_exp = expR;

        unsigned __int128 D = (unsigned __int128)4 * (unsigned __int128)i
                                     * (unsigned __int128)(i - 1);

        /* Reduce the fraction rhs_num / D. */
        unsigned __int128 g = gcd_uint128(rhs_num, D);
        unsigned __int128 num_cur_u128 = rhs_num / g;
        unsigned __int128 D2_u128      = D / g;   /* power of two */

        /* Compute how many bits are left in the denominator. */
        unsigned long long D2 = (unsigned long long)D2_u128;   /* fits 64 bits */
        int log2_D2 = __builtin_ctzll(D2);

        int exp_cur = rhs_exp + log2_D2;

        /* Store the current term for the next iteration. */
        seq_t num_cur = (seq_t)num_cur_u128;   /* truncation is OK if output overflows */

        num_nm2 = num_nm1;
        exp_nm2 = exp_nm1;
        num_nm1 = num_cur;
        exp_nm1 = exp_cur;
    }

    return num_nm1;
}
