/*
 * A098393  Prime(n)+Log2(Log2(prime(n))), where Log2=A000523.
 * https://oeis.org/A098393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) + floor(log2(floor(log2(prime(n)))))
 * Range:     u8 n <= 54, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   floor_log2_uint64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here floor(log2) is the binary logarithm (A000523). */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* safety for zero or negative indices */

    seq_t p = (seq_t)get_prime(n);           /* nth prime (1‑based) */

    /* first binary logarithm of the prime */
    unsigned l1 = floor_log2_uint64((idx_t)p);

    /* second binary logarithm (log2(log2(p))) */
    unsigned l2 = (l1 > 0) ? floor_log2_uint64((idx_t)l1) : 0;

    return p + (seq_t)l2;
}
