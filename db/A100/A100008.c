/*
 * A100008  Number of unitary divisors of 2n.
 * https://oeis.org/A100008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9785 (2026-11-01)
 * Helpers:   omega_idx_to_u32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A100008: Number of unitary divisors of 2n
    a(n) = 2^(ω(2n))  where ω(m) = number of distinct prime factors of m.
    Since ω(2n) = ω(n)  if n is even, and ω(n) + 1 if n is odd,
    we have a(n) = 2^(ω(n))  when n is even,
    a(n) = 2^(ω(n)+1)      when n is odd. */
#include <stdbool.h>
#include "../utils/omega_idx_to_u32.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, but keep harmless */

    /* number of distinct primes of n */
    unsigned int cnt = omega_idx_to_u32(n);

    /* add 2 for the prime factor 2 coming from 2n when n is odd */
    if ((n & 1) != 0) cnt++;

    /* compute 2^cnt */
    seq_t res = 1;
    for (unsigned int i = 0; i < cnt; ++i)
        res = res * 2;

    return res;
}
