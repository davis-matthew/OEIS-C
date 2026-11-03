/*
 * A217157  a(n) is the least value of k such that the decimal expansion of
 *          n^k contains two consecutive identical digits.
 * https://oeis.org/A217157
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9159 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = least k such that n^k has two consecutive identical decimal digits
   (computed by successive exponentiation and digit scanning). */

#include <stdint.h>
#include <stdbool.h>

/* Return true if the decimal number (stored little‑endian) has two equal
   adjacent digits when read in normal order. */
static bool has_consecutive(const uint8_t *digits, int len)
{
    for (int i = len - 1; i > 0; --i)
        if (digits[i] == digits[i-1]) return true;
    return false;
}

/* Multiply a decimal number (stored as little‑endian digits) by a small integer.
   digits[] contains the least significant digit first.  len is the number of
   digits (in use).  n is the multiplier (idx_t up to 64 bits). */
static void mul_by_small(uint8_t *digits, int *len, uint64_t n)
{
    /* use an unsigned 128‑bit type when available */
    typedef unsigned __int128 u128;
    u128 carry = 0;
    for (int i = 0; i < *len; ++i) {
        u128 prod = (u128)digits[i] * (u128)n + carry;
        digits[i] = (uint8_t)(prod % 10u);
        carry    = prod / 10u;
    }
    while (carry > 0) {
        digits[*len] = (uint8_t)(carry % 10u);
        carry        = carry / 10u;
        ++(*len);
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* definition applies only for n >= 2 */

    /* decimal digits stored little‑endian (least significant first) */
    uint8_t digits[200] = {0};
    int len = 1;                  /* initialise to 1 (number 1) */
    digits[0] = 1;

    /* Theoretical bound: a(n) <= 16 for n > 2, but we allow a few more steps. */
    const int MAX_K = 32;
    for (int k = 1; k <= MAX_K; ++k) {
        mul_by_small(digits, &len, n);
        if (has_consecutive(digits, len))
            return (seq_t)k;
    }

    /* Should never happen under the conjecture; return 0 as a safe fallback. */
    return 0;
}
