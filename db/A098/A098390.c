/*
 * A098390  Prime(n)+Log2(prime(n)), where Log2=A000523.
 * https://oeis.org/A098390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) + floor(log2(prime(n)))
 * Range:     u8 n <= 53, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   floor_log2_uint64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* n=0 is undefined – return 0 safely */

    uint32_t p32 = get_prime(n);   /* nth prime (1‑based) */
    seq_t   p   = (seq_t)p32;      /* cast to seq_t for the calculation */
    unsigned l2  = floor_log2_uint64((idx_t)p);

    return p + (seq_t)l2;
}
