/*
 * A032835  Numbers whose set of base-11 digits is {3,4}.
 * https://oeis.org/A032835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 62, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A032835: numbers whose base‑11 digits are only 3 and 4 */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the length L of the number by accumulating
       the counts 2^1, 2^2, 2^3, … until the cumulative
       count reaches or passes n. */
    idx_t cumulative = 0;   /* sum of 2^k for k < L */
    idx_t pow2 = 2;         /* 2^L */
    idx_t L = 1;            /* current length */

    while (cumulative + pow2 < n) {
        cumulative += pow2;
        pow2 <<= 1;
        ++L;
    }

    /* Index of the desired number within all length‑L numbers
       (0‑based). */
    idx_t index_within = n - cumulative - 1;

    /* Convert the binary representation of index_within into the
       required base‑11 number.  The most significant bit corresponds
       to the most significant digit.  0 -> digit 3, 1 -> digit 4. */
    seq_t value = 0;
    for (idx_t i = 0; i < L; ++i) {
        unsigned bit = (index_within >> (L - 1 - i)) & 1U;
        value = value * 11 + (seq_t)(3 + bit);
    }

    return value;
}
