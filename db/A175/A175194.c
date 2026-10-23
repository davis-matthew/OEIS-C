/*
 * A175194  a(n) = the smallest factorial such that (the n-th prime)+a(n) is
 *          prime, or -1 if no such prime exists.
 * https://oeis.org/A175194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{ m! : p_n + m! is prime }, where p_n is the n‑th
 *            prime, and returns -1 if no such factorial exists
 * Range:     u8 n <= 37, u16 n <= 41, u32 n <= 41, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"       /* get_prime(idx_t k) -> uint32_t */
#include "../utils/is_prime_big.h"    /* is_prime_big(seq_t n) -> bool */

seq_t a(idx_t n)
{
    /* Get the n‑th prime (1‑based). */
    seq_t prime = (seq_t)get_prime(n);

    /* Iterate over factorials m! with m starting at 1. */
    seq_t fact = (seq_t)1;     /* 1! = 1 (also covers 0! = 1). */
    idx_t m = (idx_t)1;

    /* Search up to a reasonable bound; overflow on 64‑bit is detected. */
    for (idx_t max_m = (idx_t)30; m <= max_m; ++m) {
        /* Test if prime + factorial is prime. */
        seq_t s = prime + fact;
        if (is_prime_big(s))
            return fact;

        /* Prepare next factorial: (m+1)! = m! * (m+1). */
        if (m == max_m)            /* no more iterations */
            break;
        idx_t next_m = m + 1;
        seq_t next_fact = fact * (seq_t)next_m;

        /* Detect overflow for unsigned 64‑bit types. */
        if (next_fact / (seq_t)next_m != fact)
            break;                /* overflow – assume no further valid factorials */

        fact = next_fact;
    }

    /* No factorial found within bound – return -1 as unsigned value. */
    return (seq_t)-1;
}
