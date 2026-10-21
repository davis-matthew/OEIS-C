/*
 * A204120  Symmetric matrix based on f(i,j) = gcd(prime(i+1),prime(j+1)), by
 *          antidiagonals.
 * https://oeis.org/A204120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 5724, u16 n <= 9967, u32 n <= 9967, u64 n <= 9967,
 *            u128 n <= 9967, big any n
 * Helpers:   find_antidiagonal, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A204120:  a(n) = gcd(prime(i+1), prime(j+1)) for the (i,j) th
   cell of the infinite matrix, flattened along antidiagonals.
   Off‑diagonal entries are 1, diagonal entries are odd primes. */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/find_antidiagonal.h"

/* Main sequence routine. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;      /* undefined for offset 1, safety only */

    /* Identify antidiagonal t and position within it. */
    idx_t t        = find_antidiagonal(n);              /* 1‑based diagonal index */
    idx_t prev_sum = (t - 1) * t / 2;                   /* cumulative count up to t-1 */
    idx_t offset   = n - prev_sum - 1;                  /* 0‑based offset on diagonal */
    idx_t i        = offset + 1;                        /* row index (1‑based) */
    idx_t j        = t - offset;                        /* column index (1‑based) */

    if (i != j)
        return (seq_t)1;      /* off‑diagonal */

    /* Diagonal: return the (i+1)-th prime (prime(2)=3, prime(3)=5, …). */
    return (seq_t)get_prime(i + 1);
}
