/*
 * A083440  Smallest n-digit palindrome with a digit sum of n.
 * https://oeis.org/A083440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): smallest n‑digit palindrome with digit sum n. */
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for offset 0 */
    if (n == 1) return 1;          /* a(1) = 1 */

    idx_t half = n / 2;            /* floor(n/2) */
    idx_t remainingPairs = (half > 0) ? half - 1 : 0;
    int odd = (n & 1);

    /* temporary storage for digits */
    uint8_t *d = (uint8_t *)malloc(n * sizeof(uint8_t));
    if (!d) return 0;              /* allocation failure */

    d[0] = 1;                      /* leading digit (must be >0) */
    idx_t remaining = n - 2;        /* sum left to distribute */

    /* assign digits for the outer half (excluding first digit) */
    for (idx_t i = 1; i < half; ++i) {
        idx_t inner = remainingPairs - i;
        idx_t maxRemain = 2 * 9 * inner + (odd ? 9 : 0);
        uint8_t chosen = 0;
        for (uint8_t val = 0; val <= 9; ++val) {
            if (remaining < 2 * (idx_t)val) continue;
            idx_t remAfter = remaining - 2 * (idx_t)val;
            if (remAfter <= maxRemain) {
                chosen = val;
                remaining = remAfter;
                break;
            }
        }
        d[i] = chosen;
    }

    /* middle digit for odd length */
    if (odd) {
        d[half] = (uint8_t)remaining;
        remaining = 0;
    }

    /* mirror the digits to form the palindrome */
    for (idx_t i = 1; i < half; ++i) {
        d[n - 1 - i] = d[i];
    }
    d[n - 1] = d[0];   /* last digit = first digit */

    /* build the integer value */
    seq_t res = 0;
    for (idx_t i = 0; i < n; ++i) {
        res = res * 10 + (seq_t)d[i];
    }

    free(d);
    return res;
}
