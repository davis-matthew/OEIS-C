/*
 * A212495  Numbers all of whose base 11 digits are even.
 * https://oeis.org/A212495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number whose base‑11 representation uses only even
 *            digits. Constructed by writing (n‑1) in base‑6, replacing each
 *            base‑6 digit d with 2·d, and interpreting the resulting digits
 *            (still in base‑6) as a base‑11 number
 * Range:     u8 n <= 42, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) /* safety for n < 1, though not part of the OEIS offset */
        return 0;

    idx_t idx = n - 1;     /* position in the sequence starting from 0 */
    seq_t value = 0;       /* result to return */
    seq_t power = 1;       /* current 11^position factor */

    while (idx > 0) {
        idx_t d = idx % 6; /* base‑6 digit (0…5) */
        idx /= 6;
        value += ((seq_t)(d * 2)) * power; /* map to even base‑11 digit and add */
        power *= 11; /* advance to next digit position */
    }

    return value;
}
