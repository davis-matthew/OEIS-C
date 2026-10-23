/*
 * A033037  Numbers all of whose base 12 digits are odd.
 * https://oeis.org/A033037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the number obtained by writing n-1 in base‑6, mapping
 *            each base‑6 digit d to the odd base‑12 digit 2*d+1, then
 *            interpreting the result as a base‑12 integer
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the number of base‑6 digits (length L) that cover the index n.
       There are 6^L numbers with L base‑6 digits.  We find the smallest
       L such that the cumulative count of (6^1 + 6^2 + … + 6^L) ≥ n. */
    idx_t cumulative = 0;  /* numbers with fewer than L digits */
    idx_t pow6 = 6;        /* 6^1 initially */
    uint32_t L = 1;
    while (cumulative + pow6 < n) {
        cumulative += pow6;
        ++L;
        pow6 *= 6;
    }

    /* Position inside the block of length L (0‑based) */
    idx_t pos = n - cumulative - 1;

    /* Allocate an array to hold the base‑6 digits of pos, most significant first */
    uint8_t *digits = (uint8_t *)malloc(L * sizeof *digits);
    for (uint32_t i = 0; i < L; ++i) {
        digits[L - 1 - i] = (uint8_t)(pos % 6);
        pos /= 6;
    }

    /* Convert the base‑6 digits to the base‑12 number with only odd digits. */
    seq_t result = 0;
    for (uint32_t i = 0; i < L; ++i) {
        seq_t base12digit = 2 * (seq_t)digits[i] + 1;  /* 2*d + 1 maps 0→1,1→3,…,5→11 */
        result = result * 12 + base12digit;
    }

    free(digits);
    return result;
}
