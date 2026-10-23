/*
 * A194129  Number of digits in n^100.
 * https://oeis.org/A194129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of decimal digits of n^100
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   digit_count_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute n^100 using a small arbitrary‑precision routine and count
 * its decimal digits.  All arithmetic is integer only. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/digit_count_big.h"

static const uint32_t BASE = 1000000000U;        /* 1e9, fits in 32 bits */

/* Multiply a big integer in base 1e9 by a 64‑bit small integer.
 * d   - array of digits (least significant first)
 * len - current number of used limbs; updated after multiplication
 * mul - multiplier */
static void mul_by_uint64(uint32_t *d, size_t *len, uint64_t mul)
{
    unsigned __int128 carry = 0;
    for (size_t i = 0; i < *len; ++i) {
        unsigned __int128 prod = (unsigned __int128)d[i] * mul + carry;
        d[i] = (uint32_t)(prod % BASE);
        carry = prod / BASE;
    }
    while (carry) {
        d[*len] = (uint32_t)(carry % BASE);
        carry /= BASE;
        ++*len;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)  /* 0^100 = 0 has one digit */
        return (seq_t)1;

    /* Fixed maximum number of limbs needed for 2^64-1 ^ 100
       (at most about 2000 decimal digits -> 2000/9 ≈ 223 limbs). */
    uint32_t d[256];
    size_t len = 1;
    d[0] = 1;                     /* start from 1 */

    /* Compute n^100 by repeated multiplication with small integer n. */
    for (int i = 0; i < 100; ++i)
        mul_by_uint64(d, &len, (uint64_t)n);

    int digits = digit_count_big(d, len);
    return (seq_t)digits;
}
