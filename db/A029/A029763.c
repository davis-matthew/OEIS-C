/*
 * A029763  Denominator of Bernoulli(2n+2) - Bernoulli(2n).
 * https://oeis.org/A029763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(0) = 6; for n >= 1 * a(n) = ∏_{prime p ≥ 5, (p-1)|(2n) or
 *            (p-1)|(2n+2)} p (product over odd primes > 3)
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* cached prime list */
static bool primes_initialized = false;
static uint32_t *primes = NULL;   /* array of primes up to LIMIT */
static size_t   primes_count = 0; /* number of primes stored */

/* simple sieve that creates all primes up to LIMIT */
static void init_primes(void)
{
    const idx_t LIMIT = 20050;   /* enough for n up to about 10 000 */

    /* allocate flag array */
    bool *is_composite = (bool*)calloc((size_t)(LIMIT + 1), sizeof(bool));
    if (!is_composite) return;               /* allocation failed */

    /* Eratosthenes sieve */
    for (idx_t i = 2; i * i <= LIMIT; ++i)
    {
        if (!is_composite[i])
        {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_composite[j] = true;
        }
    }

    /* count primes to allocate space */
    size_t cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (!is_composite[i]) ++cnt;

    primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!primes) { free(is_composite); return; }

    /* store primes */
    size_t pos = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (!is_composite[i]) primes[pos++] = (uint32_t)i;

    free(is_composite);
    primes_count = cnt;
    primes_initialized = true;
}

seq_t a(idx_t n)
{
    if (!primes_initialized) init_primes();

    if (n == 0) return (seq_t)6;  /* special case */

    idx_t two_n  = n * 2;
    idx_t two_n2 = two_n + 2;     /* 2*(n+1) */

    seq_t result = 1;

    /* iterate over primes, skipping 2 and 3 which always cancel for n >= 1 */
    for (size_t i = 0; i < primes_count; ++i)
    {
        uint32_t p = primes[i];
        if (p < 5) continue;     /* ignore 2 and 3 */

        idx_t d = (idx_t)p - 1;   /* p-1 must divide 2n or 2n+2 */

        if (d > two_n2) break;   /* further primes cannot satisfy condition */

        if ((two_n % d == 0) || (two_n2 % d == 0))
            result *= (seq_t)p;
    }

    return result;
}
