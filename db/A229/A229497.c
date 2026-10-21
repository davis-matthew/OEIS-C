/*
 * A229497  Product between n-th prime and next perfect square.
 * https://oeis.org/A229497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = A000040(n) * (floor_sqrt(A000040(n)) + 1)^2
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the product of the n‑th prime and the next perfect square after that prime. */
/* Requires ../utils/get_prime.h and ../utils/floor_sqrt.h */

#include "../utils/get_prime.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* n is 1‑based; caller ensures n >= 1 */

    seq_t prime = get_prime(n);          /* A000040(n) */
    seq_t s     = floor_sqrt(prime);     /* floor(sqrt(p)) */
    seq_t sq    = (s + (seq_t)1) * (s + (seq_t)1);  /* (s+1)^2 */

    return prime * sq;                   /* product */
}
