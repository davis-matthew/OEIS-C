/*
 * A066005  Sum of digits of 11^n.
 * https://oeis.org/A066005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of decimal digits of 11ⁿ
 * Range:     u8 n <= 49, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state retained across calls */
    static uint8_t *digits = NULL;   /* little‑endian decimal digits of 11ⁿ */
    static seq_t *sums = NULL;      /* stored digit sums for each n */
    static size_t digitsLen = 0;     /* number of digits currently stored */
    static size_t sumsLen = 0;       /* number of sums stored (==1 + lastComputed) */
    static size_t sumsCap = 0;       /* current capacity of sums array */
    static idx_t lastComputed = 0;  /* largest n computed so far */

    /* initialise on first call */
    if (digits == NULL) {
        digits = (uint8_t*)malloc(sizeof(uint8_t));
        digits[0] = 1;          /* 11⁰ = 1 */
        digitsLen = 1;

        sumsCap = 1;
        sums = (seq_t*)malloc(sizeof(seq_t));
        sums[0] = (seq_t)1;     /* digit sum of 1 */
        sumsLen = 1;
        lastComputed = 0;
    }

    /* if requested n already computed, return it */
    if (n <= lastComputed) {
        return sums[(size_t)n];
    }

    /* compute successive powers up to n */
    for (idx_t k = lastComputed + 1; k <= n; ++k) {
        /* multiply current number by 11 */
        uint64_t carry = 0;
        for (size_t i = 0; i < digitsLen; ++i) {
            uint64_t prod = (uint64_t)digits[i] * 11 + carry;
            digits[i] = (uint8_t)(prod % 10);
            carry = prod / 10;
        }
        while (carry > 0) {
            /* grow digits array for new most‑significant digit */
            digits = (uint8_t*)realloc(digits, (digitsLen + 1) * sizeof(uint8_t));
            digits[digitsLen++] = (uint8_t)(carry % 10);
            carry /= 10;
        }

        /* compute digit sum of the new value */
        seq_t sum = 0;
        for (size_t i = 0; i < digitsLen; ++i) {
            sum += digits[i];
        }

        /* store the sum, expanding array if necessary */
        if (sumsLen == sumsCap) {
            sumsCap = sumsCap ? sumsCap * 2 : 1;
            sums = (seq_t*)realloc(sums, sumsCap * sizeof(seq_t));
        }
        sums[sumsLen++] = sum;
    }

    lastComputed = n;
    return sums[(size_t)n];
}
