/*
 * A117673  a(n) is the least k such that k*2*prime(n) + 1 is prime.
 * https://oeis.org/A117673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k ≥ 1 such that k * 2 * prime(n) + 1 is prime
 * Range:     u8 n <= 10, u16 n <= 1477, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* n is an unsigned 64‑bit index starting at 1 */
    uint32_t p = get_prime(n);        /* the n‑th prime, offset 1 */
    seq_t target = (seq_t)p * 2;       /* 2 * prime(n) */
    seq_t k = 1;                       /* search k = 1, 2, … */

    for (;; ++k) {
        seq_t candidate = target * k + 1; /* k * 2 * prime(n) + 1 */
        if (isPrimeGeneric(candidate))
            return k;                     /* minimal k found */
    }
}
