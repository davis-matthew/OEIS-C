/*
 * A168141  a(n) = pi(n + 1) - pi(n - 2), where pi is the prime counting
 *          function.
 * https://oeis.org/A168141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(n + 1) – pi(n – 2)
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 9059 (2026-11-01)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/prime_pi.h"

/* Computes the n‑th term of sequence A168141. */
seq_t a(idx_t n)
{
    /* a(n) is defined for n ≥ 1.  For safety, return 0 for n == 0. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* pi(n + 1) */
    idx_t up = n + 1;
    seq_t pi_up = prime_pi(up);

    /* pi(n – 2) – for n <= 2 the argument is <= 0, and
       prime_pi treats n < 2 as 0. */
    idx_t down = (n > 2) ? (n - 2) : (idx_t)0;
    seq_t pi_down = prime_pi(down);

    return pi_up - pi_down;
}
