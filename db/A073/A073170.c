/*
 * A073170  a(1) = a(2) = 0; for n>2, a(n) = prime(n-1)-n+1.
 * https://oeis.org/A073170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n-1) - n + 1, a(1) = a(2) = 0
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided get_prime helper (1‑indexed) to obtain the (n‑1)th prime. */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Handle the offset explicitly */
    if (n <= 2)
        return (seq_t)0;

    /* get_prime(idx_t k) returns the k‑th prime (1‑indexed).  */
    uint32_t p = get_prime(n - 1);           /* (n‑1)th prime */
    return (seq_t)p - (seq_t)n + (seq_t)1;   /* prime(n‑1) – n + 1 */
}
