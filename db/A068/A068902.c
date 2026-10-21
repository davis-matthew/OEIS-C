/*
 * A068902  Least multiple of n not less than the n-th prime.
 * https://oeis.org/A068902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * ceil(prime(n)/n)
 * Range:     u8 n <= 51, u16 n <= 6454, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Offset is 1 – for n == 0 the sequence is undefined, return 0 */
    if (n == 0U)
        return (seq_t)0;

    /* Convert index to seq_t */
    seq_t n_seq = (seq_t)n;

    /* Fetch the n‑th prime.  get_prime works for idx_t arguments. */
    uint32_t p32 = get_prime(n);
    seq_t p_seq = (seq_t)p32;

    /* Compute ceil(p_seq / n_seq) as (p + n - 1) / n */
    seq_t q = (p_seq + n_seq - 1) / n_seq;

    return n_seq * q;
}
