/*
 * A153781  Numbers n such that n^2+13n+23 is prime.
 * https://oeis.org/A153781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 226, u32 n <= 62, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A153781: numbers n such that n^2+13n+23 is prime */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/* Return the nth element (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* the offset is 1, but guard against 0 */
    idx_t found = 0;
    idx_t candidate = 0;

    while (found < n)
    {
        seq_t val = (seq_t)candidate * ((seq_t)candidate + 13) + 23;
        if (is_prime_big(val))
        {
            found++;
            if (found == n) return (seq_t)candidate;
        }
        candidate++;
    }
    /* unreachable, but keep compiler happy */
    return 0;
}
