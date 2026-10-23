/*
 * A131841  Additive persistence of Woodall numbers.
 * https://oeis.org/A131841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = A031286(A003261(n))
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the additive persistence of the Woodall number n·2ⁿ − 1.  */
/* Implementation uses decimal digit arrays to avoid overflow.       */
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special case: n < 1 is undefined in OEIS, return 0 */
    if (n < 1) return (seq_t)0;

    /* Allocate a buffer for decimal digits (least‑significant first). */
    size_t cap = 64;
    uint8_t *digits = (uint8_t *)malloc(cap);
    if (!digits) return (seq_t)0;  /* allocation failure */
    size_t sz = 1;                 /* current number of digits */
    digits[0] = 1;                 /* value = 1  (initially 2⁰ = 1) */

    /* Compute 2ⁿ */
    for (idx_t i = 0; i < n; ++i) {
        uint16_t carry = 0;
        for (size_t j = 0; j < sz; ++j) {
            uint16_t prod = digits[j] * 2 + carry;
            digits[j] = prod % 10;
            carry    = prod / 10;
        }
        while (carry) {
            if (sz == cap) { cap *= 2; digits = (uint8_t *)realloc(digits, cap); }
            digits[sz++] = carry % 10;
            carry /= 10;
        }
    }

    /* Multiply by n: now value = n·2ⁿ */
    uint64_t nn = (uint64_t)n;
    uint64_t carry64 = 0;
    for (size_t j = 0; j < sz; ++j) {
        uint64_t prod = (uint64_t)digits[j] * nn + carry64;
        digits[j] = prod % 10;
        carry64   = prod / 10;
    }
    while (carry64) {
        if (sz == cap) { cap *= 2; digits = (uint8_t *)realloc(digits, cap); }
        digits[sz++] = carry64 % 10;
        carry64 /= 10;
    }

    /* Subtract 1: Woodall number = n·2ⁿ - 1 */
    size_t k = 0;
    while (digits[k] == 0) {
        digits[k] = 9;
        ++k;
    }
    digits[k] -= 1;
    /* Remove any leading zero (happens only when n = 1 → W = 1) */
    while (sz > 1 && digits[sz - 1] == 0) --sz;

    /* If the number is a single digit, persistence is 0. */
    if (sz == 1) {
        free(digits);
        return (seq_t)0;
    }

    /* Compute the first digit sum (W ➜ sum of its digits). */
    long long s = 0;
    for (size_t j = 0; j < sz; ++j) s += digits[j];

    /* Count reductions until a single digit remains. */
    int persistence = 1;           /* first reduction already performed */
    while (s > 9) {
        long long t = 0;
        while (s > 0) { t += s % 10; s /= 10; }
        s = t;
        ++persistence;
    }

    free(digits);
    return (seq_t)persistence;
}
