/*
 * A066781  a(n) = 2^phi(n).
 * https://oeis.org/A066781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^phi(n)
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 36, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

/* Compute Euler’s totient for an unsigned 64‑bit integer. */
static uint64_t phi_uint64(uint64_t n)
{
    if (n <= 1) return n;
    uint64_t result = n;
    uint64_t tmp = n;

    /* handle factor 2 */
    if ((tmp & 1) == 0) {
        result -= result >> 1;          /* result -= result / 2 */
        while ((tmp & 1) == 0)
            tmp >>= 1;                  /* tmp /= 2 */
    }

    /* odd factors */
    for (uint64_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            while (tmp % p == 0)
                tmp /= p;
            result -= result / p;
        }
    }

    /* remaining prime factor > sqrt(n) */
    if (tmp > 1)
        result -= result / tmp;

    return result;
}

/* External function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, but safe guard */

    uint64_t phi_val = phi_uint64((uint64_t)n);
    return pow2((idx_t)phi_val);    /* 2^phi(n) */
}
