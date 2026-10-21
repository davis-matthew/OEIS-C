/*
 * A227991  Highest power of 3 dividing prime(n)+1.
 * https://oeis.org/A227991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A227991: a(n) = 3^v_3(prime(n)+1),  n ≥ 1
   The exponent v_3(m) is the exponent of 3 in the prime factorisation of m. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/pow3.h"

/* Return the n‑th term of A227991.  For n == 0 the function returns 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined; safe default */

    /* Get the n‑th prime.  get_prime(1) is 2. */
    uint64_t p = get_prime(n);

    /* Compute the exponent of 3 in p+1.  The division loop uses seq_t
       operations only. */
    seq_t x = (seq_t)p + (seq_t)1;
    idx_t exp = 0;
    while (x % (seq_t)3 == (seq_t)0) {
        x /= (seq_t)3;
        ++exp;
    }

    /* Return the corresponding power of 3. */
    return pow3(exp);               /* 3^exp as seq_t */
}
