/*
 * A033039  Numbers all of whose base 14 digits are odd.
 * https://oeis.org/A033039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 4466, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers all of whose base‑14 digits are odd. */

#include <stdint.h>              /* for UINT64_MAX */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safety for out‑of‑range indices */

    /* Determine the length ℓ of the base‑14 representation. */
    idx_t remaining = n;
    idx_t length    = 1;
    idx_t pow7      = 7;            /* 7^length */

    while (remaining > pow7) {
        remaining -= pow7;
        ++length;
        /* Prevent overflow of pow7 on 64‑bit builds. */
        if (pow7 > UINT64_MAX / 7U) {
            pow7 = UINT64_MAX;
        } else {
            pow7 *= 7U;
        }
    }

    /* remaining is now the 1‑based index within the ℓ‑digit block. */
    idx_t idx_zero = remaining - 1; /* zero‑based */

    seq_t result  = 0;
    seq_t factor  = 1;              /* 14^position */

    for (idx_t i = 0; i < length; ++i) {
        idx_t digit   = idx_zero % 7U;   /* base‑7 digit (0..6) */
        idx_zero /= 7U;
        seq_t odd_digit = (seq_t)digit * 2U + 1U;   /* map to 1,3,…,13 */
        result += odd_digit * factor;
        factor *= (seq_t)14U;
    }

    return result;
}
