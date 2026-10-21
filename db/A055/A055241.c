/*
 * A055241  Smallest base in which n is not divisible by any of its digits (0
 *          if no such base).
 * https://oeis.org/A055241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = smallest base b≥2 such that for every digit d in the
 *            base‑b representation of n, if d≠0 then n mod d ≠ 0; return 0 if
 *            no such base exists
 * Range:     u8 n <= 425, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the base recursively or iteratively. */
#include <stdint.h>
#include <stdbool.h>

/* Computes a(n) for a given index n. */
seq_t a(idx_t n)
{
    /* For n = 0 or 1, no base can satisfy the condition. */
    if (n < 2)
        return (seq_t)0;

    /* Work with a seq_t copy of n for modulo operations. */
    seq_t n_val = (seq_t)n;

    /* Try bases from 2 up to n-1.  Base n gives a single digit n which always
       divides n, so no need to test it. */
    for (idx_t b = 2; b < n; ++b)
    {
        bool ok = true;
        idx_t temp = n;           /* integer value for generating digits */
        /* Extract digits of n in base b via repeated division. */
        while (temp)
        {
            idx_t digit = temp % b;
            if (digit != 0 && (n_val % (seq_t)digit == 0))
            {
                ok = false;
                break;
            }
            temp /= b;
        }

        if (ok)
            return (seq_t)b;      /* first base that works is the answer */
    }

    return (seq_t)0;               /* no such base found */
}
