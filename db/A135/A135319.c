/*
 * A135319  a(n) is the first digit after the decimal point in the decimal
 *          expansion of log_10(n), i.e., of the Briggsian logarithm of n.
 * https://oeis.org/A135319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = floor(10*(log_10(n) - floor(log_10(n))))
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   digits_of_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/digits_of_uint32.h"

/* Base for a simple big‑integer representation (fits in 32 bits). */
static const uint32_t BASE = 1000000000U;     /* 1e9 */

/* Count decimal digits of a 64‑bit integer. */
static uint32_t digits_of_uint64(uint64_t x)
{
    if (x >= 1000000000000000000ULL) return 19;
    if (x >= 100000000000000000ULL)  return 18;
    if (x >= 10000000000000000ULL)   return 17;
    if (x >= 1000000000000000ULL)    return 16;
    if (x >= 100000000000000ULL)     return 15;
    if (x >= 10000000000000ULL)      return 14;
    if (x >= 1000000000000ULL)       return 13;
    if (x >= 100000000000ULL)        return 12;
    if (x >= 10000000000ULL)         return 11;
    if (x >= 1000000000ULL)          return 10;
    if (x >= 100000000ULL)           return 9;
    if (x >= 10000000ULL)            return 8;
    if (x >= 1000000ULL)             return 7;
    if (x >= 100000ULL)              return 6;
    if (x >= 10000ULL)               return 5;
    if (x >= 1000ULL)                return 4;
    if (x >= 100ULL)                 return 3;
    if (x >= 10ULL)                  return 2;
    if (x >= 1ULL)                   return 1;
    return 0;
}

/* Multiply a big integer (little‑endian limbs in base BASE) by a
   64‑bit integer m.  len is updated to the new number of limbs. */
static void mul_by_uint64(uint32_t *limbs, size_t *len, uint64_t m)
{
    unsigned __int128 prod;
    unsigned __int128 carry = 0;
    size_t i = 0;
    for (; i < *len; ++i) {
        prod = (unsigned __int128)limbs[i] * m + carry;
        limbs[i] = (uint32_t)(prod % BASE);
        carry    = prod / BASE;
    }
    while (carry > 0) {
        limbs[i++] = (uint32_t)(carry % BASE);
        carry     /= BASE;
    }
    *len = i;
}

/* Compute the decimal digit count of n^10 using a small big integer. */
static uint32_t digit_count_n_pow_10(uint64_t n)
{
    /* Maximum limbs: 10 * digits(n) ≤ 200 for n up to 2^64-1,
       so 25 limbs are enough. */
    uint32_t limbs[25];
    size_t len = 0;

    /* Encode n in base BASE. */
    limbs[len++] = (uint32_t)(n % BASE);
    uint64_t carry = n / BASE;
    if (carry != 0) {
        limbs[len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
    if (carry != 0) {
        limbs[len++] = (uint32_t)carry;
    }

    /* Multiply 9 more times to get n^10. */
    for (size_t i = 0; i < 9; ++i) {
        mul_by_uint64(limbs, &len, n);
    }

    /* Count digits: most significant limb may have fewer than 9 digits. */
    uint32_t ms = limbs[len - 1];
    uint32_t ms_digits = digits_of_uint32(ms);
    return (uint32_t)((len - 1) * 9 + ms_digits);
}

/* Main OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset <1, but harmless */

    uint32_t d = digits_of_uint64(n);          /* digits(n) */
    uint32_t d10 = digit_count_n_pow_10(n);     /* digits(n^10) */

    /* a(n) = digits(n^10) - 10*digits(n) + 9  */
    int32_t result = (int32_t)d10 - (int32_t)10 * (int32_t)d + 9;
    return (seq_t)result;
}
