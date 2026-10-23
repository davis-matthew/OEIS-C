/*
 * A008683  Möbius (or Moebius) function mu(n). mu(1) = 1; mu(n) = (-1)^k if n
 *          is the product of k different primes; otherwise mu(n) = 0.
 * https://oeis.org/A008683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  core, sign, easy, mult, nice, changed
 * Formula:   a(n) = Möbius function μ(n). a(1)=1; a(n)=(-1)^k for square‑free
 *            n with k distinct prime factors, otherwise a(n)=0
 * Range:     i8 n <= 9589, i16 n <= 9589, i32 n <= 9589, i64 n <= 9589,
 *            i128 n <= 9589, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    idx_t tmp = n;
    int prime_cnt = 0;

    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            ++prime_cnt;
            tmp /= p;
            if (tmp % p == 0) return (seq_t)0;  /* square factor detected */
            while (tmp % p == 0) tmp /= p;       /* remove higher powers */
        }
    }

    if (tmp > 1) ++prime_cnt;  /* remaining prime factor > sqrt(n) */

    return (prime_cnt & 1) ? (seq_t)(-1) : (seq_t)(1);
}
