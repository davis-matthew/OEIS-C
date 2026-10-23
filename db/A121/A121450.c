/*
 * A121450  Expansion of (theta_4(q^3)^2 - theta_4(q)^2)/4 in powers of q.
 * https://oeis.org/A121450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) is multiplicative with a(2^e) = -1, a(3^e) = (-1)^e, a(p^e)
 *            = (1+(-1)^e)/2 if p ≡ 3 (mod 4), a(p^e) = e+1 if p ≡ 1 (mod 4)
 *            (p prime)
 * Range:     i8 n <= 5000, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* static cache for smallest prime factors */
static idx_t _spf_limit = 0;
static uint64_t *_spf = NULL;

/* build SPF array up to [0, n] using the classic sieve of Eratosthenes */
static void _build_spf(idx_t n)
{
    /* reallocate the array if needed */
    if (_spf_limit < n)
    {
        if (_spf) free(_spf);
        _spf = (uint64_t *)malloc((n + 1) * sizeof(uint64_t));
        if (!_spf) abort();          /* not enough memory */
        _spf_limit = n;
    }

    /* initialise: spf[i] = i for i >= 2, 0 for 0,1 */
    _spf[0] = _spf[1] = 0;
    for (idx_t i = 2; i <= n; ++i) _spf[i] = i;

    /* sieve: only factor composites */
    for (idx_t i = 2; (idx_t)((unsigned long long)i * i) <= n; ++i)
    {
        if (_spf[i] == i)                 /* i is prime */
        {
            for (idx_t j = i * i; j <= n; j += i)
            {
                if (_spf[j] == j)         /* not yet set */
                    _spf[j] = i;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* out of offset; never used */

    /* initialise SPF cache if needed */
    if (n > _spf_limit) _build_spf(n);

    seq_t result = 1;
    idx_t m = n;

    while (m > 1)
    {
        uint64_t p = _spf[m];
        idx_t e = 0;
        while (m % p == 0)
        {
            m /= p;
            ++e;
        }

        seq_t factor;
        if (p == 2)
            factor = -1;
        else if (p == 3)
            factor = (e & 1) ? -1 : 1;
        else if ((p & 3ULL) == 3ULL)   /* p ≡ 3 (mod 4) */
        {
            if (e & 1) return 0;       /* odd exponent -> zero */
            factor = 1;
        }
        else                           /* p ≡ 1 (mod 4) */
            factor = (seq_t)(e + 1);

        result *= factor;
    }

    return result;
}
