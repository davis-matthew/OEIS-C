/*
 * A055071  Largest square dividing n!.
 * https://oeis.org/A055071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p ≤ n} p^(2·⌊vp(n!)/2⌋) largest square dividing n! (vp
 *            = exponent of prime p in n!)
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static idx_t current_max = 0;          /* largest n already sieved */
static idx_t *primes = NULL;           /* array of primes up to current_max */
static size_t prime_count = 0;         /* number of primes stored */

/* Ensure that all primes ≤ n are known; recompute if a larger n is requested. */
static void ensure_primes(idx_t n)
{
    if (n <= current_max) return;

    size_t sz = (size_t)(n + 1);
    unsigned char *is_comp = (unsigned char*)calloc(sz, 1);
    if (!is_comp) exit(1);

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; (idx_t)(i * i) <= n; ++i)
        if (!is_comp[i])
            for (idx_t j = i * i; j <= n; j += i)
                is_comp[j] = 1;

    /* Count primes to allocate array */
    idx_t count = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (!is_comp[i]) ++count;

    idx_t *new_primes = (idx_t*)realloc(primes, count * sizeof(idx_t));
    if (!new_primes) exit(1);
    primes = new_primes;
    prime_count = (size_t)count;

    size_t idx = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (!is_comp[i]) primes[idx++] = i;

    free(is_comp);
    current_max = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 1;              /* safe default for illegal inputs */

    ensure_primes(n);

    seq_t result = 1;

    for (size_t i = 0; i < prime_count && primes[i] <= n; ++i)
    {
        idx_t p = primes[i];
        /* Legendre's formula to find exponent of p in n! */
        idx_t vp = 0;
        idx_t power = p;
        while (power <= n)
        {
            vp += n / power;
            if (power > n / p) break; /* avoid overflow */
            power *= p;
        }

        /* Highest even exponent not exceeding vp */
        idx_t exp = (vp / 2) * 2;
        for (idx_t k = 0; k < exp; ++k)
            result = result * (seq_t)p;
    }

    return result;
}
