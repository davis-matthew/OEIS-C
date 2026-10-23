/*
 * A201305  Primes of the form 10n^3+7.
 * https://oeis.org/A201305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*k^3 + 7 where k is the n‑th integer such that 10*k^3+7
 *            is prime
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime of the form 10*k^3 + 7 by brute‑force search. */
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n) {
    /* Count primes found so far. */
    idx_t count = 0;
    /* Current integer k. */
    idx_t k = 0;
    while (1) {
        /* Compute 10*k^3 + 7 using 128‑bit arithmetic to avoid overflow. */
        __int128 val128 = (__int128)10 * k * k * k + 7;
        seq_t val = (seq_t)val128;
        /* Test primality. */
        if (is_prime_seq(val)) {
            ++count;
            if (count == n) {
                return val;
            }
        }
        ++k;
    }
}
