/*
 * A216244  a(n) = (prime(n)^2 - 1)/2 for n >= 2.
 * https://oeis.org/A216244
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(n)^2 - 1)/2 for n >= 2
 * Range:     u8 n <= 8, u16 n <= 72, u32 n <= 4000, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The helper function get_prime(k) returns the k‑th prime (1‑based).
   Its declaration is provided by "../utils/get_prime.h". */
#include "../utils/get_prime.h"
#include <stdint.h>

/* Computes the n‑th term of OEIS A216244.  For n < 2 the function
   returns 0; the sequence is defined only for n >= 2.  All
   calculations use a 128‑bit intermediate to avoid overflow on
   64‑bit builds. */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    /* Retrieve the n‑th prime as a 32‑bit unsigned integer. */
    uint32_t p = get_prime(n);

    /* Compute (p^2 - 1)/2 using a 128‑bit intermediate. */
    unsigned __int128 sq = (unsigned __int128)p * p;
    sq = (sq - 1) / 2;

    return (seq_t)sq;
}
