/*
 * A068633  a(1) = 11 by convention; for n > 1, if n = p^a*q^b... then a(n) =
 *          concatenate(p,a,q,b,...).
 * https://oeis.org/A068633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 29, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A068633: a(1)=11; for n>1, let n=∏pᵃ, then a(n)=concatenation(p,a,…) in increasing prime order. */
/* This implementation uses trial‑division factorisation, which is sufficient for n up to ~10⁶ (our tests are ≤10⁴). */

#include <stdint.h>
#include <stddef.h>

/* Count decimal digits of a non‑zero uint64_t. */
static inline int digits_uint64(uint64_t x)
{
    int d = 0;
    do { x /= 10; ++d; } while (x);
    return d;
}

/* Return 10ⁿ as seq_t (multiplication is safe for the intended ranges). */
static inline seq_t pow10_seq(int e)
{
    seq_t r = 1;
    while (e--) r *= (seq_t)10;
    return r;
}

/* Factorisation by simple trial division.
   The function assumes n>=2.  It stores the primes and
   exponents in the arrays passed by the caller. */
static void factor_uint64(uint64_t n, uint64_t *primes, uint64_t *exps, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p <= n / p; ++p)
    {
        if (n % p == 0)
        {
            uint64_t e = 0;
            while (n % p == 0)
            {
                n /= p;
                ++e;
            }
            primes[c] = p;
            exps[c]   = e;
            ++c;
        }
    }
    if (n > 1)
    {
        primes[c] = n;
        exps[c]   = 1ULL;
        ++c;
    }
    *cnt = c;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)11;      /* convention */

    uint64_t primes[32];
    uint64_t exps[32];
    size_t    cnt;
    factor_uint64((uint64_t)n, primes, exps, &cnt);

    seq_t res = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p = (seq_t)primes[i];
        seq_t e = (seq_t)exps[i];
        res = res * pow10_seq(digits_uint64(primes[i])) + p;
        res = res * pow10_seq(digits_uint64(exps[i]))   + e;
    }
    return res;
}
