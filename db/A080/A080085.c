/*
 * A080085  Number of factors of 2 in the factorial of the n-th prime, counted
 *          with multiplicity.
 * https://oeis.org/A080085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k≥1} ⌊p_n / 2^k⌋ where p_n is the n‑th prime
 *            (Legendre’s formula)
 * Range:     u8 n <= 55, u16 n <= 6543, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

/* Computes the exponent of 2 in the prime factorisation of (p_n)!,
   where p_n is the n‑th prime. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined for n<1, return 0 for safety */

    uint32_t p = get_prime(n);          /* n‑th prime (fits in 32 bits) */
    seq_t total = 0;
    uint64_t tmp = (uint64_t)p;         /* shift on 64‑bit to avoid overflow */

    while (tmp)
    {
        tmp >>= 1;
        total += (seq_t)tmp;            /* add ⌊p / 2^k⌋ for this k */
    }
    return total;
}
