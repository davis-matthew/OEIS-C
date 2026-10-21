/*
 * A057860  Number of residue classes modulo n which contain only composite
 *          numbers.
 * https://oeis.org/A057860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n - φ(n) - [n is prime] - ω'(n) where φ(n) is Euler's
 *            totient, [n is prime] is 1 if n is prime else 0, and ω'(n)
 *            counts distinct prime factors of n that are strictly less than
 *            n. This matches the OEIS definition of A057860
 * Range:     u8 n <= 359, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime64.h"

seq_t a(idx_t n) {
    /* Handle the trivial case n == 1 directly */
    if (n == 1) {
        return 0;
    }

    uint64_t un = n;          /* local 64‑bit copy of the index */
    seq_t phi = (seq_t)n;     /* Euler's totient, will be reduced */
    uint64_t omega = 0;       /* distinct prime factors < n */

    uint64_t temp = un;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            ++omega;                              /* new distinct prime factor */
            phi = phi / (seq_t)p * ((seq_t)p - 1); /* φ = φ/p*(p-1) */
            while (temp % p == 0) {
                temp /= p;
            }
        }
    }
    if (temp > 1) {          /* remaining prime factor */
        if (temp < un) {
            ++omega;
        }
        phi = phi / (seq_t)temp * ((seq_t)temp - 1);
    }

    seq_t result = (seq_t)un - phi;          /* subtract φ(n) */
    if (is_prime64(un)) {
        result -= 1;                         /* subtract contribution of m = 0 */
    }
    result -= (seq_t)omega;                  /* subtract prime class factors */

    return result;
}
