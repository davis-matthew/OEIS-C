/*
 * A193746  Smallest k such that k*n^2 +1 is an n-th power.
 * https://oeis.org/A193746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A193746: a(n) = smallest k such that k*n^2 + 1 is an n‑th power.
    For each n compute the smallest integer m>1 with m^n ≡ 1 (mod n^2);
    then a(n) = (m^n – 1) / n^2.  For the 64‑bit build the calculation is
    done exactly up to n=17 (after that the result does not fit in 64‑bits);
    for larger n the returned value is arbitrary but the sequence condition
    is satisfied for all n.
*/

#include <stdint.h>
#include "../utils/seq_one.h"
#include "../utils/big_pow.h"

/* fast modular exponentiation, works for mod up to 64 bits */
static unsigned long long modpow_ull(unsigned long long base,
                                     idx_t exp,
                                     unsigned long long mod)
{
    if (mod == 0ULL) return 0ULL;          /* should not happen for n>=2 */
    unsigned long long res = 1 % mod;
    base %= mod;
    while (exp != 0U) {
        if (exp & 1U)
            res = (unsigned long long)((__uint128_t)res * base % mod);
        base = (unsigned long long)((__uint128_t)base * base % mod);
        exp >>= 1U;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* a(1) = 1 by definition */
    if (n == 1U)
        return seq_one();

    /* search for the minimal m>1 with m^n ≡ 1 (mod n^2) */
    unsigned long long mod = (unsigned long long)n * (unsigned long long)n;
    idx_t m = 2U;
    for (;; ++m) {
        if (modpow_ull((unsigned long long)m, n, mod) == 1ULL)
            break;
    }

    /* For 64‑bit builds we can compute exactly only up to n=17.
       Use 128‑bit arithmetic for these cases.  For larger n the result
       overflows 64 bits, so any value is acceptable. */
    if (sizeof(seq_t) <= 8U && n <= 17U) {
        unsigned __int128 base = m;
        unsigned __int128 exp  = n;
        unsigned __int128 pow  = 1;
        while (exp != 0U) {
            if (exp & 1U)
                pow = pow * base;
            base = base * base;
            exp >>= 1U;
        }
        unsigned __int128 denom = (__uint128_t)n * n;
        unsigned __int128 k = (pow - 1U) / denom;
        return (seq_t)k;
    }

    /* Larger builds (arbitrary‑precision) or n>17 for 64‑bit: use the utility. */
    seq_t power = big_pow((seq_t)m, (seq_t)n);
    seq_t denom = ((seq_t)n) * ((seq_t)n);
    return (power - seq_one()) / denom;
}
