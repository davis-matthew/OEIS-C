/*
 * A083251  Numbers m such that abs(A045763(m) - A073757(m)) = 2, i.e., signed
 *          difference of size of related and unrelated sets to n equals
 *          either 2 or -2.
 * https://oeis.org/A083251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 2, a(2) = 48, a(3) = 72; for n ≥ 4: a(n) = 16 *
 *            prime(n‑1) (prime(1)=2, prime(2)=3, …)
 * Range:     u8 n <= 7, u16 n <= 565, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* special cases for the first three terms */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)48;
    if (n == 3) return (seq_t)72;

    /* n >= 4: compute the (n‑1)‑st prime and multiply by 16 */
    /* get_prime(k) returns the k-th prime (k >= 1). */
    uint32_t p = get_prime((idx_t)(n - 1));
    return (seq_t)16u * (seq_t)p;
}
