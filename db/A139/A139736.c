/*
 * A139736  a(n) = 10^n mod 6^n.
 * https://oeis.org/A139736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10^n mod 6^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

/* Compute base^exp mod mod using binary exponentiation.
   Handles the special case mod == 1 (any integer mod 1 is 0). */
static seq_t mod_pow_big(seq_t base, idx_t exp, seq_t mod)
{
    if (mod == 1) return 0;
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1ULL) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t mod3n   = seq_pow((seq_t)3, n);          /* 3^n   */
    seq_t pow5mod = mod_pow_big((seq_t)5, n, mod3n);/* 5^n mod 3^n */
    seq_t pow2n   = seq_pow((seq_t)2, n);          /* 2^n   */
    return pow2n * pow5mod;                       /* a(n)  */
}
