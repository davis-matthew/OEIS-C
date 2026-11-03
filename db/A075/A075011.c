/*
 * A075011  Floor[ concatenation of numbers from n to 1 divided by
 *          concatenation of numbers from 1 to n].
 * https://oeis.org/A075011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = floor( concat(n, n-1, …, 1) / concat(1, 2, …, n) )
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 116 terms through n = 975 (2026-11-01)
 * Helpers:   num_digits_u32_to_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds the two large concatenated numbers in base‑10
   using small digit arrays and repeatedly subtracts until the remainder is
   smaller than the denominator.  The quotient is guaranteed to be at most
   9, so a simple subtraction loop suffices for all n */
#include <stddef.h>
#include <stdint.h>
#include "../utils/num_digits_u32_to_i32.h"

/* add a small unsigned integer (up to 32 bits) to a little‑endian number */
static void bigint_add_small(uint8_t *digits, size_t *len, uint32_t x)
{
    uint8_t small[10];
    size_t small_len = 0;
    if (x == 0) { small[0] = 0; small_len = 1; }
    else {
        while (x) { small[small_len++] = x % 10; x /= 10; }
    }

    size_t i = 0;
    uint32_t carry = 0;
    while (i < small_len || carry) {
        if (i >= *len) { digits[i] = 0; ++*len; }
        uint32_t sum = digits[i] + (i < small_len ? small[i] : 0) + carry;
        digits[i] = (uint8_t)(sum % 10);
        carry   = sum / 10;
        ++i;
    }
}

/* compare two little‑endian decimal numbers: a>b -> 1, a==b -> 0, a<b -> -1 */
static int bigint_cmp(const uint8_t *a, size_t alen, const uint8_t *b, size_t blen)
{
    if (alen > blen) return 1;
    if (alen < blen) return -1;
    for (long long i = (long long)alen - 1; i >= 0; --i) {
        if (a[i] > b[i]) return 1;
        if (a[i] < b[i]) return -1;
    }
    return 0;
}

/* multiply a little‑endian decimal number by 10^d (prepend d zeros)   */
static void bigint_shift(uint8_t *digits, size_t *len, int d)
{
    if (d == 0) return;
    for (long long i = (long long)*len - 1; i >= 0; --i)
        digits[i + d] = digits[i];
    for (int i = 0; i < d; ++i) digits[i] = 0;
    *len += d;
}

/* subtract b from a (a >= b).  Result stored back in a.                 */
static void bigint_sub(uint8_t *a, size_t *alen, const uint8_t *b, size_t blen)
{
    int borrow = 0;
    for (size_t i = 0; i < *alen; ++i) {
        int diff = (int)a[i] - (i < blen ? (int)b[i] : 0) - borrow;
        if (diff < 0) { diff += 10; borrow = 1; }
        else          { borrow = 0;           }
        a[i] = (uint8_t)diff;
    }
    while (*alen > 0 && a[*alen - 1] == 0) --*alen;
}

/* maximum number of decimal digits needed for n up to 10 000 (≈38894)   */
#define MAX_DIGITS 40000

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Big integers are stored as arrays of decimal digits (least
       significant digit first).  The total length never exceeds MAX_DIGITS.   */
    uint8_t a_digits[MAX_DIGITS] = {0};
    size_t   a_len = 0;
    uint8_t b_digits[MAX_DIGITS] = {0};
    size_t   b_len = 0;

    /* Build A = concat(1 .. n) */
    for (idx_t i = 1; i <= n; ++i) {
        int d = num_digits_u32_to_i32((uint32_t)i);
        bigint_shift(a_digits, &a_len, d);
        bigint_add_small(a_digits, &a_len, (uint32_t)i);
    }

    /* Build B = concat(n .. 1) */
    for (idx_t i = n; i >= 1; --i) {
        int d = num_digits_u32_to_i32((uint32_t)i);
        bigint_shift(b_digits, &b_len, d);
        bigint_add_small(b_digits, &b_len, (uint32_t)i);
    }

    /* Compute floor(B / A) by repeated subtraction */
    seq_t q = 0;
    while (bigint_cmp(b_digits, b_len, a_digits, a_len) >= 0) {
        bigint_sub(b_digits, &b_len, a_digits, a_len);
        ++q;
    }

    return q;
}
