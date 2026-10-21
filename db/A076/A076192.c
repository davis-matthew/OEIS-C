/*
 * A076192  n == 1 mod 10 with property that n through n+9 contain no primes.
 * https://oeis.org/A076192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computes the n‑th integer of OEIS A076192. * A076192 : the
 *            smallest number n ≡ 1 (mod 10) such that the * decade [n, n+9]
 *            contains no prime numbers. * The function returns the n‑th such
 *            number, for n ≥ 1
 * Range:     u8 n <= 1, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"  // provides is_prime_big(seq_t)

seq_t a(idx_t n) {
    /* For n = 0 we return 0, though the sequence is defined for n >= 1. */
    if (n == 0) return (seq_t)0;

    seq_t candidate = (seq_t)1;   /* we will add 10 each loop */
    idx_t count = 0;

    while (1) {
        /* move to the next number ending in 1 */
        candidate += 10;

        /* test the ten consecutive numbers for primality */
        bool decade_ok = true;
        for (int i = 0; i < 10 && decade_ok; ++i) {
            seq_t v = candidate + (seq_t)i;
            if (is_prime_big(v)) decade_ok = false;
        }

        if (decade_ok) {
            ++count;
            if (count == n) return candidate;
        }
    }
}
