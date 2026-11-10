/*
 * A084484  a(n) = A007088(A084483(n)).
 * https://oeis.org/A084484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007088(A084483(n)); A084483 edits the rightmost zero
 *            block of n (delete one 0 if its length is even, else insert
 *            one), and A007088 reads the binary digits as a decimal number
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 511, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 54 terms through n = 9827 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A084483_a(idx_t n)
{
    if (n == 0) return 0;
    idx_t t = n;
    unsigned k = 0;
    while ((t & 1ULL) == 1ULL) { t >>= 1; k++; }
    if (t == 0) return n;
    unsigned len = 0;
    while ((t & 1ULL) == 0ULL) { t >>= 1; len++; }
    unsigned len2 = (len & 1U) ? (len + 1U) : (len - 1U);
    return (t << (len2 + k)) + ((((idx_t)1) << k) - 1ULL);
}

static seq_t A007088_a(idx_t n)
{
    seq_t result = 0;
    seq_t power_of_10 = 1;
    while (n > 0) {
        if (n & 1) result += power_of_10;
        n >>= 1;
        power_of_10 *= 10;
    }
    return result;
}

seq_t a(idx_t n)
{
    return A007088_a(A084483_a(n));
}
