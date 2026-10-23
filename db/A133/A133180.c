/*
 * A133180  a(n) = (Sum_{k=1..A047380(n)} k^6) / (Sum_{k=1..A047380(n)} k^2).
 * https://oeis.org/A133180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*m^4 + 6*m^3 - 3*m + 1) / 7, where m = 7 * ((n - 1) /
 *            4) + offset[(n - 1) % 4] and offset = {1, 2, 4, 5}
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const idx_t OFFSET[4] = {1, 2, 4, 5};

seq_t a(idx_t n)
{
    /* Compute m = A047380(n), the n-th integer ≡ 1,2,4,5 (mod 7) */
    idx_t q = (n - 1) / 4;               /* block number (starting from 0)  */
    idx_t r = (n - 1) % 4;               /* position inside the block       */
    seq_t m = (seq_t)7 * (seq_t)q + (seq_t)OFFSET[r];

    /* Compute a(n) = (3*m^4 + 6*m^3 - 3*m + 1) / 7  */
    seq_t m2 = m * m;                    /* m^2         */
    seq_t m3 = m2 * m;                   /* m^3         */
    seq_t m4 = m3 * m;                   /* m^4         */

    seq_t num = (seq_t)3 * m4
              + (seq_t)6 * m3
              - (seq_t)3 * m
              + (seq_t)1;

    return num / (seq_t)7;
}
