/*
 * A202708  Sum of digits of n! divided by 9.
 * https://oeis.org/A202708
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = digit_sum(n!)/9 for n ≥ 6 (n! is the usual factorial)
 * Range:     u8 n <= 276, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_sum_big.h"

/* Base 10^9 representation.  Each element holds up to 9 decimal digits. */
#define BASE     1000000000U
#define BASE_DIG 9U

/* Multiply big integer a[0..len-1] (little‑endian) by small integer m. */
static void mul_uint32(uint32_t *a, size_t *len, uint32_t m)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < *len; ++i) {
        uint64_t prod = (uint64_t)a[i] * m + carry;
        a[i] = (uint32_t)(prod % BASE);
        carry  = prod / BASE;
    }
    while (carry) {
        a[(*len)++] = (uint32_t)(carry % BASE);
        carry      /= BASE;
    }
}

/* Return (sum of digits of n!) / 9.  For n < 6 the result is undefined;
   we return 0 in that case. */
seq_t a(idx_t n)
{
    if (n < 6) return 0;

    /* Allocate a buffer large enough for the maximum number of base‑BASE digits
       of n! (for n up to 10000 that's < 4000 digits). */
    const size_t MAX_DIGITS = 4500;   /* safety margin */
    uint32_t *buf = (uint32_t *)malloc(MAX_DIGITS * sizeof(uint32_t));
    if (!buf) return 0;  /* allocation failure → 0 */

    size_t len = 1;
    buf[0] = 1;                     /* start with 1! = 1 */

    /* Compute factorial: multiply by 2,3,...,n. */
    for (idx_t i = 2; i <= n; ++i) {
        mul_uint32(buf, &len, (uint32_t)i);
    }

    /* Compute digit sum and divide by 9. */
    uint64_t sum = digit_sum_big(buf, len);
    free(buf);
    return (seq_t)(sum / 9);
}
