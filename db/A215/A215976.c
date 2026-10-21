/*
 * A215976  2-adic valuation of the denominator of sum( k!/2^k, k=1..n ).
 * https://oeis.org/A215976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2-adic valuation of the denominator of sum_{k=1}^n k! /
 *            2^k
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* offset is 1, but make function safe */

    /* step 1: find M = max popcount in [1..n] */
    unsigned M = 0;
    for (idx_t k = 1; k <= n; ++k) {
        unsigned s = __builtin_popcountll((unsigned long long)k);
        if (s > M)
            M = s;
    }

    /* step 2: prepare modulus 2^M and mask (mod-1) */
    /* M is small (≤ about 14 for n ≤ 10 000), so the shift is safe. */
    unsigned long long maskULL = (M == 0) ? 0ULL : ((1ULL << M) - 1ULL);

    seq_t f = 1;   /* odd part of k! modulo 2^M */
    seq_t T = 0;   /* accumulated sum modulo 2^M */

    for (idx_t k = 1; k <= n; ++k) {
        /* remove all 2s from k to get its odd part */
        uint64_t temp = k;
        while ((temp & 1ULL) == 0ULL)
            temp >>= 1ULL;

        /* f = f * temp (mod 2^M) */
        unsigned long long prod = (unsigned long long)f * temp;
        f = (seq_t)(prod & maskULL);

        /* add the term k! / 2^{v2(k!)} * 2^{M - popcount(k)} */
        unsigned s = __builtin_popcountll((unsigned long long)k);
        unsigned shift = M - s;
        unsigned long long termProd = (unsigned long long)f * ((1ULL << shift) & maskULL);
        seq_t term = (seq_t)(termProd & maskULL);

        T += term;
        T &= maskULL;
    }

    /* step 3: determine v2(T) */
    unsigned v = 0;
    if (T == (seq_t)0) {
        v = M;   /* T is divisible by 2^M */
    } else {
        seq_t tmp = T;
        while ((tmp & 1) == 0) {
            tmp >>= 1;
            ++v;
        }
    }

    unsigned a_val = M - v;
    return (seq_t)a_val;
}
