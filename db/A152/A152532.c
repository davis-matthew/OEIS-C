/*
 * A152532  a(n) = prime(n) * prime(n+2) - 2 * prime(n+1).
 * https://oeis.org/A152532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n) * prime(n+2) - 2 * prime(n+1)
 * Range:     u8 n <= 6, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided get_prime utility (1‑indexed primes). */
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 by OEIS definition */
    seq_t p_n  = (seq_t)get_prime(n);
    seq_t p_n1 = (seq_t)get_prime(n + 1);
    seq_t p_n2 = (seq_t)get_prime(n + 2);

    return p_n * p_n2 - (seq_t)2 * p_n1;
}
