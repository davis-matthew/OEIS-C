/*
 * A091738  Primes arising in the second row of array in A091734.
 * https://oeis.org/A091738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(6*floor((n-1)/2)+2 + 2*((n-1)%2))
 * Range:     u8 n <= 18, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where prime(k) is the k-th prime (1‑based). */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; no checks for n<1 are required by the spec. */
    idx_t m = (n - 1) / 2;          /* floor((n-1)/2)                       */
    int r = (n - 1) % 2;            /* parity of n-1 (0 or 1)               */
    idx_t idx = 6 * m + 2 + 2 * r;  /* index of the desired prime (1‑based) */
    return (seq_t) get_prime(idx);
}
