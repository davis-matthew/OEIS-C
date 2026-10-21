/*
 * A079684  Number of 3's in n!.
 * https://oeis.org/A079684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   number of decimal digit '3' in n! (OEIS A079684)
 * Range:     u8 n <= 988, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Count of digit '3' in the decimal representation of n! */
    if (n < 2)  /* 0! = 1, 1! = 1 – no '3' */
        return (seq_t)0;

    /* Allocate space for the factorial in base 10^9.  The number of
       blocks needed for n ≤ 10000 is < n*5 (≈5000).  This is safe
       for the required values and keeps reallocations unnecessary. */
    size_t cap = (size_t)(n * 5ULL) + 10;
    uint32_t *blk = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!blk) return (seq_t)0;   /* fallback on allocation failure */

    size_t len = 1;          /* number of used blocks */
    blk[0] = 1;              /* start from 1! = 1 */
    const uint64_t BASE = 1000000000ULL;  /* 10^9 */

    /* Multiply 2 … n, keeping the number in base BASE. */
    for (idx_t i = 2; i <= n; ++i) {
        uint64_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            uint64_t tmp = (uint64_t)blk[j] * i + carry;
            blk[j] = (uint32_t)(tmp % BASE);
            carry = tmp / BASE;
        }
        while (carry > 0) {
            blk[len++] = (uint32_t)(carry % BASE);
            carry /= BASE;
        }
    }

    /* Count '3's in the decimal digits of the factorial. */
    unsigned long long count = 0;  /* fits in any seq_t for n ≤ 10000 */
    for (size_t idx = len; idx > 0; --idx) {
        uint32_t v = blk[idx - 1];
        if (idx == len) {  /* most significant block – no leading zeros */
            if (v == 0) continue;  /* should not happen */
            while (v > 0) {
                if (v % 10 == 3) ++count;
                v /= 10;
            }
        } else {  /* other blocks – always 9 decimal digits (with leading zeros) */
            for (int d = 0; d < 9; ++d) {
                if (v % 10 == 3) ++count;
                v /= 10;
            }
        }
    }

    free(blk);
    return (seq_t)count;
}
