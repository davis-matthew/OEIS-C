/*
 * A098386  a(n) = prime(n)-Log2(n), where Log2 = A000523.
 * https://oeis.org/A098386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000040(n) - A000523(n) (prime(n) - floor(log2(n)))
 * Range:     u8 n <= 55, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   floor_log2_uint64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1, so n must be >= 1.  Guard against 0 just in case. */
    if (n < 1)
        return (seq_t)0;

    seq_t prime = (seq_t)get_prime(n);            /* A000040(n) */
    seq_t log2  = (seq_t)floor_log2_uint64(n);    /* A000523(n) */

    return prime - log2;
}
