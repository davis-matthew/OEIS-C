/*
 * A069114  Squarefree part of prime(n)-1 : the smallest number such that
 *          a(n)*(prime(n)-1) is a square.
 * https://oeis.org/A069114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = squarefree part of prime(n)-1 (the smallest number a such
 *            that a*(prime(n)-1) is a perfect square)
 * Range:     u8 n <= 55, u16 n <= 6544, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/smallest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* sequence is defined for n ≥ 1 */

    /* Get the n-th prime (1‑based).  get_prime(n) returns a uint32_t,
       but we cast it to uint64_t for safety. */
    uint64_t p = (uint64_t)get_prime(n);   /* prime(n) */
    uint64_t m = p - 1;                    /* prime(n) - 1 */

    uint64_t tmp = m;          /* temporary number to factor */
    seq_t     res = 1;         /* result accumulates square‑free part */

    while (tmp > 1) {
        /* Smallest prime factor of tmp */
        uint64_t spf = smallest_prime_factor(tmp);

        uint64_t exp = 0;
        while (tmp % spf == 0) {
            tmp /= spf;
            exp++;
        }

        /* Multiply only if exponent is odd (square‑free part) */
        if (exp & 1ULL)
            res = res * (seq_t)spf;
    }

    return res;
}
