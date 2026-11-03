/*
 * A240676  Number of digits in the decimal expansion of n^7.
 * https://oeis.org/A240676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of decimal digits in the decimal expansion of n^7
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 68 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Multiply a big integer, stored in base 1e9, by a 64‑bit unsigned integer.
   The number is stored in little‑endian order, least significant limb first. */
static void big_mul_uint64(uint32_t *digits, size_t *len, uint64_t m)
{
    const uint64_t BASE = 1000000000ULL; /* 1e9 */
    uint64_t carry = 0;
    for (size_t i = 0; i < *len; ++i) {
        uint64_t prod = (uint64_t)digits[i] * m + carry;
        digits[i] = (uint32_t)(prod % BASE);
        carry     = prod / BASE;
    }
    while (carry > 0) {
        digits[*len] = (uint32_t)(carry % BASE);
        carry /= BASE;
        ++(*len);
    }
}

/* Count decimal digits of a 32‑bit integer in the [0,10^9) range */
static unsigned digit_len_uint32(uint32_t v)
{
    if (v >= 100000000U) return 9;
    if (v >= 10000000U)  return 8;
    if (v >= 1000000U)   return 7;
    if (v >= 100000U)    return 6;
    if (v >= 10000U)     return 5;
    if (v >= 1000U)      return 4;
    if (v >= 100U)       return 3;
    if (v >= 10U)        return 2;
    return 1; /* 0 … 9 */
}

/* Compute a(n) = number of decimal digits in n^7 */
seq_t a(idx_t n)
{
    /* Special case: 0^7 = 0 has one digit */
    if (n == 0) return 1;

    /* Big integer in base 1e9; 20 limbs are ample for n up to 10000^7. */
    uint32_t digits[20] = {0};
    size_t  len = 0;

    /* Initialise with the value of n. */
    uint64_t tmp = n;
    do {
        digits[len++] = (uint32_t)(tmp % 1000000000ULL);
    } while ((tmp /= 1000000000ULL) > 0);

    /* Multiply 6 more times to obtain n^7 */
    for (int i = 0; i < 6; ++i)
        big_mul_uint64(digits, &len, n);

    /* Most significant limb */
    uint32_t msd = digits[len - 1];
    unsigned msd_digits = digit_len_uint32(msd);

    return (seq_t)((len - 1) * 9 + msd_digits);
}
