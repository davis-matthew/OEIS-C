/*
 * A171622  a(n) = floor(n-th noncomposite / n).
 * https://oeis.org/A171622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(A008578(n)/n)
 * Range:     u8 n <= 55, u16 n <= 6645, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of range, return 0 for safety */
    if (n == 1) return (seq_t)1;   /* A008578(1) = 1 */

    /* For n ≥ 2, the n‑th non‑composite number equals the (n‑1)‑th prime. */
    seq_t p = (seq_t)get_prime(n - 1);
    return p / (seq_t)n;           /* integer division gives floor() */
}
