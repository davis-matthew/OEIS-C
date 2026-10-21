/*
 * A050985  Cubefree part of n.
 * https://oeis.org/A050985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product of p^(e mod 3) over all prime divisors p^e of n
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalently the largest cube–free divisor of n) */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* handle n == 0 (outside offset) – return 0 */
    if (n == 0) return 0;

    seq_t result = 1;
    uint64_t m = (uint64_t)n;
    uint64_t exp_mod;

    /* factor 2 */
    if ((m & 1) == 0) {
        uint64_t cnt = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            cnt++;
        }
        exp_mod = cnt % 3;
        if (exp_mod) {
            /* 2^1 or 2^2 */
            seq_t p = 2;
            if (exp_mod == 1) result *= p;
            else /* exp_mod==2 */ result *= (p * p);
        }
    }

    /* factor odd primes */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t cnt = 0;
            while (m % p == 0) {
                m /= p;
                cnt++;
            }
            exp_mod = cnt % 3;
            if (exp_mod) {
                seq_t base = (seq_t)p;
                if (exp_mod == 1) result *= base;
                else result *= (base * base); /* exp_mod == 2 */
            }
        }
    }

    /* remaining prime factor (if any) */
    if (m > 1) {
        /* exponent is 1 -> exp_mod = 1 */
        seq_t base = (seq_t)m;
        result *= base;
    }

    return result;
}
