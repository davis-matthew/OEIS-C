/*
 * A115777  Levenshtein distance between n considered as a decimal string and
 *          n considered as a binary string.
 * https://oeis.org/A115777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9632, u16 n <= 9632, u32 n <= 9632, u64 n <= 9632,
 *            u128 n <= 9632, big any n
 * Helpers:   binary_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the Levenshtein distance between the decimal string of n
 * and the binary string of n (OEIS A115777). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binary_digits.h"

/* Convert n to decimal digits (least‑significant first).
 * Returns the number of digits written to buf. */
static idx_t
decimal_digits(idx_t n, uint8_t *buf)
{
    if (n == 0) {
        buf[0] = 0;
        return 1;
    }
    idx_t len = 0;
    while (n > 0) {
        buf[len++] = (uint8_t)(n % 10);
        n /= 10;
    }
    return len;
}

seq_t a(idx_t n)
{
    /* Temporary digit buffers: 20 decimal digits max for 64‑bit,
     * 64 binary digits. */
    uint8_t dec[20];
    uint8_t bin[64];

    idx_t lenDec = decimal_digits(n, dec);
    idx_t lenBin = binary_digits(n, bin);

    /* Allocate two rows for the dynamic programming table. */
    size_t rowSize = (size_t)(lenBin + 1) * sizeof(idx_t);
    idx_t *prev = (idx_t *)malloc(rowSize);
    idx_t *curr = (idx_t *)malloc(rowSize);
    if (!prev || !curr) {
        /* On allocation failure, return 0 (unlikely in tests). */
        free(prev);
        free(curr);
        return 0;
    }

    /* Base case: distance from empty decimal string to binary prefix. */
    prev[0] = 0;
    for (idx_t j = 1; j <= lenBin; ++j)
        prev[j] = j;

    /* DP over each decimal digit. */
    for (idx_t i = 1; i <= lenDec; ++i) {
        curr[0] = i;                 /* delete all i decimal digits. */
        uint8_t d = dec[i - 1];      /* current decimal digit. */

        for (idx_t j = 1; j <= lenBin; ++j) {
            uint8_t b = bin[j - 1];  /* current binary digit. */
            idx_t cost = (d == b) ? 0 : 1;
            idx_t del = prev[j] + 1;     /* delete decimal digit. */
            idx_t ins = curr[j - 1] + 1; /* insert decimal digit. */
            idx_t sub = prev[j - 1] + cost;
            idx_t m = (del < ins) ? del : ins;
            m = (m < sub) ? m : sub;
            curr[j] = m;
        }
        /* Swap rows for next iteration. */
        idx_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = (seq_t)prev[lenBin];
    free(prev);
    free(curr);
    return result;
}
