/*
 * A078599  Product of squarefree divisors of n.
 * https://oeis.org/A078599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p|n} p^{2^{ω(n)-1}}, where ω(n) is the number of
 *            distinct prime factors of n Equivalently, a(n) =
 *            rad(n)^{2^{ω(n)-1}}, and a(1) = 1
 * Range:     u8 n <= 20, u16 n <= 29, u32 n <= 209, u64 n <= 329,
 *            u128 n <= 1000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 1, and a(0) is unused */
        return (seq_t)1;

    uint64_t m = (uint64_t)n;    /* factor n, 64‑bit enough for our range */
    uint64_t rad = 1;            /* product of distinct primes */
    int  d = 0;                  /* number of distinct primes */

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            rad *= p;
            ++d;
            while (m % p == 0)
                m /= p;
        }
    }
    if (m > 1) {
        rad *= m;
        ++d;
    }

    seq_t rad_s = rad;                   /* cast to result type */
    seq_t exp   = ((seq_t)1) << (d - 1); /* 2^(d-1) */

    return big_pow(rad_s, exp);          /* rad_s ^ exp */
}
