/*
 * A142355  Primes congruent to 2 mod 47.
 * https://oeis.org/A142355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 141, u32 n <= 40, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the n-th prime p with p ≡ 2 (mod 47)
 * The function searches for primes of the form 47·k+2 until the n‑th
 * one is found.  It works for any seq_t and idx_t as provided by the
 * build and does not rely on storing a table of values.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1; behaviour for n<1 is undefined */

    idx_t count = 0;                     /* primes found so far */
    idx_t k     = 0;                     /* candidate index (p = 47·k + 2) */

    while (count < n) {
        /* generate candidate prime p = 47·k + 2 */
        uint64_t tmp = 47ULL * k + 2ULL;
        seq_t p = tmp;                   /* convert to seq_t */

        if (is_prime_big(p)) {
            count++;
            if (count == n) return p;
        }
        k++;
    }

    /* should never reach here */
    return 0;
}
