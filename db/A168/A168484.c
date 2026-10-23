/*
 * A168484  Numbers that are congruent to {2, 3, 5, 7} mod 11.
 * https://oeis.org/A168484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11 * floor((n-1)/4) + {2,3,5,7}[(n-1) mod 4]
 * Range:     u8 n <= 93, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A168484: numbers congruent to 2, 3, 5, or 7 (mod 11) */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    if (n == 0u)  /* undefined for n = 0; return 0 as a safe placeholder */
        return (seq_t)0;

    /* Base residues modulo 11 */
    static const seq_t base[4] = { (seq_t)2, (seq_t)3, (seq_t)5, (seq_t)7 };

    /* Convert to 0‑based index */
    idx_t idx = n - 1u;
    idx_t q   = idx / 4u;        /* number of completed 4‑term blocks */
    idx_t r   = idx % 4u;        /* position within the current block */

    /* Compute the nth term using 11 * q + base[r] */
    return (seq_t)11 * (seq_t)q + base[(size_t)r];
}
