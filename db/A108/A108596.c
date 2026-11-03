/*
 * A108596  Numbers k such that 911*k - 7 is prime.
 * https://oeis.org/A108596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 888 (2026-11-01)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that 911*k - 7 is prime – computed by brute‑force search. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* not part of the offset: a(1) is first term */
    idx_t found = 0;
    seq_t k = 0;

    for (;;)
    {
        ++k;                          /* next candidate k */
        seq_t val = (seq_t)911 * k - (seq_t)7;
        if (is_prime_seq(val))
        {
            ++found;
            if (found == n)
                return k;
        }
    }
}
