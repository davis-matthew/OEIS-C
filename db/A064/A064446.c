/*
 * A064446  a(n) = gcd(n!, n^n, lcm(1, 2, ..., n)), or gcd(n^n, lcm(1, 2, ...,
 *          n)).
 * https://oeis.org/A064446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{prime p | n} p^floor(log_p(n))
 * Range:     u8 n <= 22, u16 n <= 69, u32 n <= 545, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = gcd(n^n, lcm(1,…,n)) for n ≥ 1 */

#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* factorize n to obtain its distinct prime divisors */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t n_seq = (seq_t)n;
    seq_t res = 1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t pow = p;        /* first power is p^1 */
        for (;;) {
            seq_t next = pow * p;
            if (next <= n_seq)
                pow = next;
            else
                break;
        }
        res *= pow;
    }

    return res;
}
