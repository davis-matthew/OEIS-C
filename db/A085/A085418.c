/*
 * A085418  Primes reached in A085417.
 * https://oeis.org/A085418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = first prime of the form p_n + ∑_{k=n}^{m} k with minimal
 *            m ≥ n
 * Range:     u8 n <= 13, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* offset 1 – behaviour for n<1 is irrelevant */

    seq_t p = get_prime(n);   /* p_n (uint32_t cast automatically to seq_t) */
    seq_t sum = 0;
    seq_t m = (seq_t)n;

    for (;; ++m) {
        sum += m;                     /* sum of consecutive integers n..m */
        seq_t cand = p + sum;         /* candidate prime */
        if (is_prime_big(cand))
            return cand;
    }
}
