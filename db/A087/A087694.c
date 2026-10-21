/*
 * A087694  Number of solutions to x^2 + xy + y^2 == 0 (mod n).
 * https://oeis.org/A087694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   multiplicative with a(3^e) = 3^e, a(p^e) = ((p-1)*e + p) *
 *            p^(e-1) if p ≡ 1 (mod 3), a(p^e) = p^(2 * floor(e/2)) if p ≡ 2
 *            (mod 3)
 * Range:     u8 n <= 90, u16 n <= 9138, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Fast exponentiation for seq_t */
static seq_t pow_seq(seq_t base, unsigned exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

/* Factorise a 64‑bit unsigned integer into primes and exponents.
   primes[i] contains the i‑th prime factor, exps[i] its exponent.
   cnt receives the number of distinct primes. */
static void factor_uint64(uint64_t n,
                          uint64_t *primes,
                          uint64_t *exps,
                          size_t *cnt)
{
    size_t c = 0;
    uint64_t m = n;

    if (m == 0 || m == 1)
    {
        *cnt = 0;
        return;
    }

    if (m % 2ULL == 0ULL)
    {
        uint64_t e = 0;
        while (m % 2ULL == 0ULL)
        {
            m /= 2ULL;
            ++e;
        }
        primes[c] = 2ULL;
        exps[c] = e;
        ++c;
    }

    for (uint64_t p = 3ULL; p * p <= m; p += 2ULL)
    {
        if (m % p == 0ULL)
        {
            uint64_t e = 0;
            while (m % p == 0ULL)
            {
                m /= p;
                ++e;
            }
            primes[c] = p;
            exps[c] = e;
            ++c;
        }
    }

    if (m > 1ULL)
    {
        primes[c] = m;
        exps[c] = 1ULL;
        ++c;
    }

    *cnt = c;
}

seq_t a(idx_t n)
{
    if (n <= 1U)
        return (seq_t)1;

    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt;

    factor_uint64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t contrib;

        if (p == 3ULL)
        {
            contrib = pow_seq((seq_t)3, (unsigned)e);
        }
        else if ((p % 3ULL) == 1ULL)
        {
            unsigned __int128 tmp128 = (unsigned __int128)(p - 1ULL) * e + p;
            seq_t tmp = (seq_t)tmp128;
            seq_t pow_part = pow_seq((seq_t)p, (unsigned)(e - 1ULL));
            contrib = pow_part * tmp;
        }
        else /* p % 3 == 2 */
        {
            uint64_t exp2 = (e / 2ULL) * 2ULL;   /* 2 * floor(e/2) */
            contrib = pow_seq((seq_t)p, (unsigned)exp2);
        }

        result *= contrib;
    }

    return result;
}
