/*
 * A018535  Divisors of 590.
 * https://oeis.org/A018535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 590: a(n) = product of primes {2,5,59} raised to the
   binary bits of (n‑1).  The sequence has exactly 8 terms
   (1,2,5,10,59,118,295,590).  For n > 8 the function returns 0. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* prime factors of 590 */
    static const idx_t primes[3] = {2, 5, 59};
    const idx_t num_primes = sizeof primes / sizeof primes[0];
    const idx_t max_n     = ((idx_t)1 << num_primes);  /* =8 */

    if (n == 0 || n > max_n)
        return 0;

    idx_t mask = n - 1;
    seq_t result = 1;

    for (size_t i = 0; i < num_primes; ++i) {
        if (mask & ((idx_t)1 << i))
            result *= (seq_t)primes[i];
    }
    return result;
}
