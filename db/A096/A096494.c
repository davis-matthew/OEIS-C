/*
 * A096494  Largest value in the periodic part of the continued fraction of
 *          sqrt(prime(n)).
 * https://oeis.org/A096494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * floor(sqrt(p(n))) where p(n) is the n-th prime. This
 *            follows from the fact that for a square‑free integer D, the
 *            continued fraction of sqrt(D) has period ending with
 *            2*floor(sqrt(D))
 * Range:     u8 n <= 18, u16 n <= 1897, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* offset is 1; undefined below */

    uint32_t prime_val = get_prime(n);  /* returns the n-th prime */
    seq_t sqr_floor   = floor_sqrt((seq_t)prime_val);
    return (seq_t)2 * sqr_floor;
}
