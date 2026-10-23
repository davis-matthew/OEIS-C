/*
 * A036527  Smallest cube containing exactly n 0's.
 * https://oeis.org/A036527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A048365(n)^3, equivalently * a(0) = 1, a(1) = 0, and for
 *            n >= 2: * if n ≡ 0 (mod 3) → a(n) = 10^n * if n ≡ 1 (mod 3) →
 *            a(n) = 4096 * 10^(n-1) * if n ≡ 2 (mod 3) → a(n) = 140608 *
 *            10^(n-2)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 7, u64 n <= 16, u128 n <= 21,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0ULL)
        return (seq_t)1;
    if (n == 1ULL)
        return (seq_t)0;

    idx_t r = n % 3ULL;
    if (r == 0ULL)          /* a(3k) = 10^(3k) = 10^n */
        return pow10(n);
    else if (r == 1ULL)     /* a(3k+1) = 4096 * 10^(3k) = 4096 * 10^(n-1) */
        return (seq_t)4096ULL * pow10(n - 1ULL);
    else                    /* r == 2: a(3k+2) = 140608 * 10^(3k) = 140608 * 10^(n-2) */
        return (seq_t)140608ULL * pow10(n - 2ULL);
}
