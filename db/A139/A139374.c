/*
 * A139374  Digit sum of Lucas numbers.
 * https://oeis.org/A139374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digit sum of the n-th Lucas number (L(0)=2, L(1)=1,
 *            L(n)=L(n-1)+L(n-2))
 * Range:     u8 n <= 234, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9697 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
static void add_decimal_digits(const uint8_t *a, size_t als,
                               const uint8_t *b, size_t bls,
                               uint8_t *out, size_t *ols)
{
    size_t i, maxlen = (als > bls ? als : bls);
    uint8_t carry = 0;
    for (i = 0; i < maxlen; ++i) {
        uint8_t da = (i < als) ? a[i] : 0;
        uint8_t db = (i < bls) ? b[i] : 0;
        uint16_t sum = (uint16_t)da + (uint16_t)db + (uint16_t)carry;
        out[i] = (uint8_t)(sum % 10);
        carry   = (uint8_t)(sum / 10);
    }
    if (carry) {
        out[i++] = carry;
    }
    while (i > 1 && out[i-1] == 0) { /* strip possible leading zeros */
        --i;
    }
    *ols = i;
}

/* ------------------------------------------------------------------ */
/* Computes the digit sum of the n-th Lucas number using decimal digits  */
/* representation (little‑endian).                                       */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) {
        return (seq_t)2;
    }
    if (n == 1) {
        return (seq_t)1;
    }

    /* Estimate maximum number of decimal digits for the n-th Lucas number */
    size_t max_digits = (size_t)((n * 210ULL) / 1000ULL + 5ULL); /* ≈ 0.21*n + 5 */

    /* Allocate buffers for three consecutive Lucas numbers */
    uint8_t *buf0 = (uint8_t *)malloc(max_digits + 1);
    uint8_t *buf1 = (uint8_t *)malloc(max_digits + 1);
    uint8_t *buf2 = (uint8_t *)malloc(max_digits + 1);
    if (!buf0 || !buf1 || !buf2) {
        /* Allocation failed – return 0 as a fallback value */
        return (seq_t)0;
    }

    /* Initialize L(0)=2 and L(1)=1 */
    size_t len0 = 1, len1 = 1;
    buf0[0] = 2;          /* L(0) */
    buf1[0] = 1;          /* L(1) */

    /* Iteratively compute Lucas numbers up to L(n) */
    for (idx_t i = 2; i <= n; ++i) {
        size_t len2;
        add_decimal_digits(buf0, len0, buf1, len1, buf2, &len2);

        /* Rotate buffers: buf0 <- buf1, buf1 <- buf2 */
        uint8_t *tmp_buf = buf0;
        size_t   tmp_len = len0;

        buf0 = buf1;
        len0 = len1;

        buf1 = buf2;
        len1 = len2;

        buf2 = tmp_buf;
        len2 = tmp_len;  /* unused later, but keeps symmetry */
    }

    /* Compute the digit sum of the final Lucas number stored in buf1 */
    size_t digit_sum = 0;
    for (size_t i = 0; i < len1; ++i) {
        digit_sum += buf1[i];
    }

    /* Clean up */
    free(buf0);
    free(buf1);
    free(buf2);

    return (seq_t)digit_sum;
}
