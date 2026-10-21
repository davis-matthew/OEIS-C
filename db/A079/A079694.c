/*
 * A079694  Number of 9's in n!.
 * https://oeis.org/A079694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of digit '9' in n! (decimal representation)
 * Range:     u8 n <= 977, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_nines
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/count_nines.h"

static const uint32_t BASE = 1000000000u;   /* 1e9, fits in uint32_t */

static void mul_small(uint32_t *a, size_t *len, uint32_t m)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < *len; ++i) {
        uint64_t prod = (uint64_t)a[i] * m + carry;
        a[i] = (uint32_t)(prod % BASE);
        carry = prod / BASE;
    }
    while (carry > 0) {
        a[*len] = (uint32_t)(carry % BASE);
        carry /= BASE;
        ++(*len);
    }
}

seq_t a(idx_t n)
{
    /* a static buffer of 5 000 limbs is enough for n up to ~10 000 */
    static uint32_t data[5000];
    size_t len = 1;
    data[0] = 1;   /* 0! = 1 */

    if (n < 2)
        return (seq_t)0;   /* 0! and 1! contain no 9 */

    for (idx_t i = 2; i <= n; ++i) {
        mul_small(data, &len, (uint32_t)i);
    }
    return count_nines(data, len);
}
