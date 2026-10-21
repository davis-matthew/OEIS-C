/*
 * A227990  3^a(n) is the highest power of 3 dividing prime(n)+1.
 * https://oeis.org/A227990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = v_3(prime(n)+1) where prime(n) is the nth prime (offset =
 *            1)
 * Range:     u8 n <= 54, u16 n <= 6695, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   // undefined for non‑positive indices

    seq_t p = (seq_t)get_prime(n);   // nth prime
    seq_t val = p + 1;               // prime + 1
    seq_t cnt = 0;                   // 3‑adic valuation

    while (val % 3 == 0) {
        val /= 3;
        cnt++;
    }

    return cnt;
}
