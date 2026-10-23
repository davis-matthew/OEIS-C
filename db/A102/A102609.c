/*
 * A102609  a(n) = 1 iff n^2-n+41 is prime (else 0).
 * https://oeis.org/A102609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 iff n^2 - n + 41 is prime, else 0
 * Range:     u8 n <= 15, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   is_prime_uint64, isprime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_uint64.h"
#include "../utils/isprime_big.h"

/* Return 1 if n^2 - n + 41 is prime, else 0. */
seq_t a(idx_t n) {
    /* Compute the value n^2 - n + 41 in seq_t. */
    seq_t val = (seq_t)n * (seq_t)n;   /* n^2 */
    val -= (seq_t)n;                  /* n^2 - n */
    val += 41;                        /* n^2 - n + 41 */

    /* Try to test primality using the 64‑bit routine if the value fits. */
    bool prime = false;
    {
        uint64_t v64 = (uint64_t)val;          /* truncate to 64 bits */
        if ((seq_t)v64 == val) {              /* value fits in 64 bits */
            prime = is_prime_uint64(v64);
        } else {                              /* use arbitrary‑precision test */
            prime = isprime_big(val);
        }
    }

    return (seq_t)(prime ? 1 : 0);
}
