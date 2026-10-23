/*
 * A024451  a(n) is the numerator of Sum_{i = 1..n} 1/prime(i).
 * https://oeis.org/A024451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, nice
 * Formula:   a(n) = (Product of the first n primes) * (Sum_{i=1..n}
 *            1/prime(i)) Recurrence: a(0) = 0 a(n) = a(n-1) * prime(n) +
 *            Product_{i=1..n-1} prime(i) for n ≥ 1 The function uses the
 *            utilities get_prime(idx_t k) : returns the k-th prime (1‑based)
 *            seq_mul(seq_t a, seq_t b) : multiplies two seq_t values
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   get_prime, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t product = (seq_t)1;  /* P_{n-1} initially P_0 = 1 */
    seq_t an = (seq_t)0;       /* a(0) */

    for (idx_t i = (idx_t)1; i <= n; ++i) {
        uint32_t p = get_prime(i);          /* prime(i) */
        seq_t temp = seq_mul(an, (seq_t)p); /* a(n-1) * prime(n) */
        an = temp + product;                /* a(n) = a(n-1)*p + P_{n-1} */
        product = seq_mul(product, (seq_t)p);/* update P_n for next iteration */
    }

    return an;
}
