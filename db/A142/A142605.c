/*
 * A142605  Primes congruent to 6 mod 55.
 * https://oeis.org/A142605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 159, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth prime p with p ≡ 6 (mod 55) (n ≥ 1).  The
   function checks numbers of the form 55*k+6 until the desired
   prime is found. */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* n == 0 is outside the OEIS offset – return 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    idx_t count = 0;
    idx_t candidate = 61;          /* 55*1 + 6, the first term of the sequence */
    const idx_t step = 55;

    while (count < n) {
        if (is_prime(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        candidate += step;
    }

    /* The loop always returns before reaching here. */
    return (seq_t)0;
}
