/*
 * A097250  Smallest m such that A097249(m) = n; from n=1 onwards, twice the
 *          primorials, 2*A002110(n).
 * https://oeis.org/A097250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = if n=0 then 1 else 2*A002110(n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   get_prime, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        uint32_t p = get_prime(i);          /* i‑th prime */
        res = mul_small(res, p);            /* res *= p */
    }
    res = mul_small(res, 2);                /* res *= 2 */
    return res;
}
