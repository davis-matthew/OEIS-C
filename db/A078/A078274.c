/*
 * A078274  a(n) is the number from which if odd-positioned digits are deleted
 *          then one gets the concatenation of first n odd numbers, while if
 *          even-positioned digits are deleted then one gets reverse
 *          concatenation of first n odd numbers. Position of least
 *          significant digit is considered to be 1.
 * https://oeis.org/A078274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   interleaving digits of the concatenation of the first n odd
 *            numbers (A019519(n)) with its reverse (A038395(n))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   digit_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "../utils/digit_count.h"

seq_t a(idx_t n)
{
    /* Compute the total number of digits of the ascending part.
       The number of digits of the descending part is the same. */
    uint64_t two_n_minus1 = (uint64_t)2 * (uint64_t)n - 1;
    size_t len_asc = 0;
    for (uint64_t m = 1; m <= two_n_minus1; m += 2)
        len_asc += (size_t)digit_count(m);
    size_t len_rev = len_asc;          /* same number of digits */

    /* Allocate arrays to hold the digits in order from most significant
       to least significant. */
    uint8_t *asc = (uint8_t *)malloc(len_asc);
    uint8_t *rev = (uint8_t *)malloc(len_rev);
    if (asc == NULL || rev == NULL) {
        free(asc);
        free(rev);
        return (seq_t)0;               /* should not happen for tested n */
    }

    /* Fill the ascending digits array. */
    size_t idx = 0;
    for (uint64_t m = 1; m <= two_n_minus1; m += 2) {
        /* Convert m to a string to process its digits from most to least. */
        char buf[32];
        int l = sprintf(buf, "%llu", (unsigned long long)m);
        for (int i = 0; i < l; ++i)
            asc[idx++] = (uint8_t)(buf[i] - '0');
    }

    /* Fill the reverse digits array. */
    idx = 0;
    for (uint64_t m = two_n_minus1;; m -= 2) {
        char buf[32];
        int l = sprintf(buf, "%llu", (unsigned long long)m);
        for (int i = 0; i < l; ++i)
            rev[idx++] = (uint8_t)(buf[i] - '0');
        if (m == 1)
            break;                     /* avoid unsigned wrap‑around */
    }

    /* Interleave the two digit arrays to form the final number. */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < len_asc; ++i) {
        result = result * (seq_t)10 + (seq_t)asc[i];
        result = result * (seq_t)10 + (seq_t)rev[i];
    }

    free(asc);
    free(rev);
    return result;
}
