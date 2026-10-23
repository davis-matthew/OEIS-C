/*
 * A032837  Numbers whose set of base-13 digits is {3,4}.
 * https://oeis.org/A032837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th base‑13 number composed solely of the digits 3
 *            and 4, sorted by increasing decimal value
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Determine the length L of the required base‑13 representation. */
    idx_t count = 0;        /* total numbers of all shorter lengths */
    idx_t power2 = 1;       /* 2^0 */
    idx_t L = 0;

    while (1) {
        L++;                       /* now consider length L */
        power2 <<= 1;              /* 2^L   */
        if (count + power2 >= n)   /* n falls inside this length */
            break;
        count += power2;           /* add all numbers of this length to count */
    }

    /* Offset within numbers of this length, zero‑based. */
    idx_t offset = n - count - 1;

    /* Build the resulting decimal value digit by digit. */
    seq_t val = 0;
    for (idx_t i = L; i-- > 0; ) {          /* from most significant digit to least */
        unsigned bit   = (offset >> i) & 1u; /* 0 → digit 3, 1 → digit 4 */
        unsigned digit = 3u + bit;
        val = val * (seq_t)13 + (seq_t)digit;
    }
    return val;
}
