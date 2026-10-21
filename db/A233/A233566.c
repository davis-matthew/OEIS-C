/*
 * A233566  a(n) = |{0 < p < n: p and p*phi(n-p) - 1 are both prime}|, where
 *          phi(.) is Euler's totient function (A000010).
 * https://oeis.org/A233566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{0 < p < n : p and p*phi(n-p) - 1 are both prime}|
 * Range:     u8 n <= 33, u16 n <= 1503, u32 n <= 1503, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, is_prime_big, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence by brute‑force search, using provided prime tests
   and an Euler totient routine. */

#include <stdbool.h>
#include "../utils/phi.h"          /* seq_t phi(idx_t m); */
#include "../utils/is_prime.h"     /* bool is_prime(idx_t x); */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t n); */

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t p = 2; p < n; ++p) {
        if (!is_prime(p))
            continue;
        seq_t phi_val = phi(n - p);
        seq_t value = ((seq_t)p) * phi_val - 1;
        if (is_prime_big(value))
            ++count;
    }
    return count;
}
