/*
 * A154412  Primes of the form 10*k^2+14*k+5, k >= 0.
 * https://oeis.org/A154412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 33, u32 n <= 35, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime of the form 10*k^2 + 14*k + 5 by brute‑force search. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* behaviour for n < 1 is irrelevant */

    uint64_t found = 0;
    for (uint64_t k = 0; ; ++k) {
        seq_t val = (seq_t)10 * k * k + (seq_t)14 * k + (seq_t)5;
        if (is_prime_big(val)) {
            ++found;
            if (found == n)
                return val;
        }
    }
}
