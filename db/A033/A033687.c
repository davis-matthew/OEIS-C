/*
 * A033687  Theta series of hexagonal lattice A_2 with respect to deep hole
 *          divided by 3.
 * https://oeis.org/A033687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b(3*n + 1), where b is multiplicative: b(p^e) = 0 if p =
 *            3, b(p^e) = e+1 if p ≡ 1 (mod 6), b(p^e) = (1+(-1)^e)/2 if p ≡ 2
 *            or 5 (mod 6) (i.e. factor is 1 when exponent is even, 0 when
 *            odd)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* Prime table for factorisation (sieves up to LIMIT). */
static uint32_t *primes    = NULL;
static size_t   primes_cnt = 0;
#define PRIME_SIEVE_LIMIT 200000u  /* enough for n up to ~10⁹ */

/* Initialise the prime list once. */
static void init_primes(void)
{
    if (primes != NULL)
        return;

    size_t limit = PRIME_SIEVE_LIMIT;
    char *sieve = (char*)calloc(limit + 1, sizeof(char));
    if (!sieve) return;           /* allocation failed – unlikely */

    for (size_t i = 2; i * i <= limit; ++i)
        if (!sieve[i])
            for (size_t j = i * i; j <= limit; j += i)
                sieve[j] = 1;

    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!sieve[i]) ++cnt;

    primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!primes) { free(sieve); return; }

    size_t pos = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!sieve[i]) primes[pos++] = (uint32_t)i;

    primes_cnt = cnt;
    free(sieve);
}

/* Compute multiplicative function b(m). */
static seq_t calc_b(idx_t m)
{
    if (m == 1)
        return 1;

    seq_t result = 1;
    idx_t temp = m;

    for (size_t p_i = 0; p_i < primes_cnt; ++p_i) {
        idx_t p = primes[p_i];
        if ( (idx_t)p * (idx_t)p > temp )
            break;
        if (temp % p != 0)
            continue;

        idx_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }

        if (p == 3)
            return 0;

        if (p % 6 == 1) {
            result *= (seq_t)(e + 1);
        } else { /* p % 6 == 2 or 5 */
            if (e & 1)      /* odd exponent kills the factor */
                return 0;
            /* even exponent contributes factor 1 – nothing to do */
        }
    }

    /* Remaining prime factor if any. */
    if (temp > 1) {
        idx_t p = temp;          /* exponent e == 1 */
        if (p == 3)
            return 0;
        if (p % 6 == 1)
            result *= (seq_t)2;   /* e+1 = 2 */
        else                 /* p % 6 == 2 or 5, odd exponent */
            return 0;
    }

    return result;
}

/* Public interface required by the framework. */
seq_t a(idx_t n)
{
    init_primes();            /* ensure prime list is ready */
    idx_t m = 3 * n + 1;
    return calc_b(m);
}
