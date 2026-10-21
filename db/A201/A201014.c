/*
 * A201014  Composite numbers (include 0) whose product of digits is 0.
 * https://oeis.org/A201014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A201014: non‑prime numbers (including 0) that contain a 0 digit. */
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Check if a number has at least one digit equal to 0. */
static bool has_zero_digit(seq_t v)
{
    /* 0 itself is handled separately in the main function. */
    while (v > 0)
    {
        if (v % (seq_t)10 == 0)
            return true;
        v /= (seq_t)10;
    }
    return false;
}

/* Return the nth term of the sequence, where n≥1. */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;          /* first term is 0 */

    idx_t count = 1;              /* count of terms found so far (includes 0) */
    idx_t num   = 1;              /* candidate number to test */

    while (1)
    {
        /* Skip primes (note: 1 is not prime but has no zero digit, so it's skipped automatically) */
        if (num > 1 && is_prime(num))
        {
            ++num;
            continue;
        }

        /* Include the number if it has a zero digit */
        if (has_zero_digit((seq_t)num))
        {
            ++count;
            if (count == n)
                return (seq_t)num;
        }
        ++num;
    }
}
