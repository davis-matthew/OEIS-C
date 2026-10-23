/*
 * A183095  a(n) = number of divisors d of n that are either 1 or of the form
 *          Product_(i) (p_i^e_i) where e_i = 1 for at least one i.
 * https://oeis.org/A183095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = τ(n) - (∏_{p^e||n, e≥2} e - 1) = τ(n) - ∏ + 1 where τ(n)
 *            = number of divisors of n and the product runs over all prime
 *            powers of n whose exponent e ≥ 2. This counts all divisiors that
 *            are NOT powerful (i.e. have at least one prime with exponent 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;          /* the number to factorise */
    seq_t tau = (seq_t)1;              /* τ(n) */
    seq_t pdiv = (seq_t)1;             /* product of e for e≥2, otherwise 1 */

    /* factor 2 */
    uint64_t cnt = 0;
    while ((x & 1ULL) == 0) {
        x >>= 1;
        cnt++;
    }
    if (cnt) {
        tau *= (seq_t)(cnt + 1);
        if (cnt >= 2)
            pdiv *= (seq_t)cnt;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            cnt = 0;
            while (x % p == 0) {
                x /= p;
                cnt++;
            }
            tau *= (seq_t)(cnt + 1);
            if (cnt >= 2)
                pdiv *= (seq_t)cnt;
        }
    }

    /* remaining prime factor, if any */
    if (x > 1) {
        tau *= (seq_t)2;      /* exponent 1 gives two choices: 0 or 1 */
        /* cnt = 1 → no change to pdiv */
    }

    /* a(n) = τ(n) - (pdiv - 1) = τ(n) - pdiv + 1 */
    return tau - pdiv + (seq_t)1;
}
