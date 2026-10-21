/*
 * A093844  n-th row of the following triangle contains n terms of an
 *          arithmetic progression with the first term 1 such that the sum is
 *          the least possible n-th power. Sequence contains the leading
 *          diagonal.
 * https://oeis.org/A093844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*m^n – n)/n, where m is the least positive integer such
 *            that m^n ≡ n (mod n*(n-1)/2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"   /* for seq_pow(seq_t, idx_t) */

static uint64_t mod_pow_uint(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1) return 0ULL;
    uint64_t result = 1ULL % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1ULL)
        {
            result = (unsigned __int128)result * base % mod;
        }
        base = (unsigned __int128)base * base % mod;
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* Compute modulus M = n*(n-1)/2 and target residue n mod M. */
    idx_t M = n * ((idx_t)(n - 1)) / 2;
    idx_t target = n % M;

    /* Search for the smallest m ≥ 1 with m^n ≡ target (mod M). */
    for (idx_t m = 1; ; ++m)
    {
        if (mod_pow_uint(m, n, M) != target) continue;

        /* Exact n-th power of m. */
        seq_t S = seq_pow((seq_t)m, n);

        /* Ensure the power is at least n (so the arithmetic progression
           has positive difference). */
        if (S < (seq_t)n) continue;

        /* Compute a(n) = (2*S - n) / n. */
        seq_t a_val = ((S * (seq_t)2) - (seq_t)n) / (seq_t)n;
        return a_val;
    }
}
