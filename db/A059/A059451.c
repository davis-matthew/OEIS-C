/*
 * A059451  Number of ways n can be written as the sum of two numbers whose
 *          binary expansions have even numbers of zeros; also number of ways
 *          n can be written as the sum of two numbers whose binary expansions
 *          have odd numbers of zeros.
 * https://oeis.org/A059451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{(x,y): 0 ≤ x ≤ y, x + y = n, #0bits(x) mod 2 = 1,
 *            #0bits(y) mod 2 = 1}|. The binary representation of 0 is taken
 *            to have one zero bit
 * Range:     u8 n <= 1278, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 111 terms through n = 9171 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool odd_zero_parity(idx_t x)
{
    if (x == 0)
        return true;                 /* 0 has one zero bit → odd */
    unsigned long long ux = (unsigned long long)x;
    unsigned int bits = 64 - __builtin_clzll(ux);      /* bit length */
    unsigned int ones = __builtin_popcountll(ux);     /* number of 1‑bits */
    /* zeros = bits - ones; odd iff parity of that is odd */
    return ((bits - ones) & 1u) != 0;
}

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    for (idx_t k = 0; k <= n / 2; ++k) {
        if (odd_zero_parity(k) && odd_zero_parity(n - k))
            ++cnt;
    }
    return cnt;
}
