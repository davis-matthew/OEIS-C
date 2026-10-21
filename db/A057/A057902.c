/*
 * A057902  a(n) = 5^prime(n).
 * https://oeis.org/A057902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5^prime(n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   get_prime, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A057902 where prime(n) is the nth prime. */

#include <stdint.h>
#include "../utils/get_prime.h"   /* get_prime(idx_t k) → k‑th prime (uint32_t) */
#include "../utils/pow_seq.h"    /* pow_seq(const seq_t base, int exp) → base^exp */

seq_t a(idx_t n)
{
    /* nth prime (1‑based). For n ≥ 1. */
    uint32_t p = get_prime(n);

    /* Compute 5^p.  pow_seq works for arbitrary‑precision seq_t. */
    return pow_seq((seq_t)5, (int)p);
}
