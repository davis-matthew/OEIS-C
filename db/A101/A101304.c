/*
 * A101304  a(n) = 2^(prime(n) + 1) + 1.
 * https://oeis.org/A101304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(prime(n) + 1) + 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   is_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the n‑th term of OEIS sequence A101304. */

#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/pow2.h"

/* Compute the n-th term of the sequence. */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1; undefined for 0 */

    uint64_t count = 0;            /* number of primes found so far   */
    uint64_t candidate = 2;        /* current integer being tested    */
    uint64_t prime_val = 0;        /* value of the n‑th prime         */

    /* Find the n‑th prime by trial using the supplied is_prime function. */
    while (count < n) {
        if (is_prime(candidate)) {
            ++count;
            if (count == n) {
                prime_val = candidate;
            }
        }
        ++candidate;
    }

    /* 2^(prime + 1) + 1 */
    seq_t result = pow2(prime_val + 1);
    result += (seq_t)1;
    return result;
}
