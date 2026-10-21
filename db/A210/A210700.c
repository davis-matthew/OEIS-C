/*
 * A210700  A047160(3n): smallest m >= 0 with both 3n - m and 3n + m prime.
 * https://oeis.org/A210700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 3333, u32 n <= 3333, u64 n <= 3333,
 *            u128 n <= 3333, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Minimal m≥0 such that 3n - m and 3n + m are primes */
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;                     /* 3*1 = 3 is prime */

    seq_t base = (seq_t)3 * (seq_t)n; /* 3n  (seq_t guarantees enough width) */

    /* For even n, m must be odd; for odd n, m must be even. */
    idx_t m = (n & 1) ? 2 : 1;        /* start value */

    for (;; m += 2)                   /* increase by 2 to keep the parity */
    {
        if (m % 3 == 0)               /* skip multiples of 3 */
            continue;

        seq_t left  = base - (seq_t)m;
        seq_t right = base + (seq_t)m;

        if (isPrimeGeneric(left) && isPrimeGeneric(right))
            return (seq_t)m;         /* found the minimal m */
    }
}
