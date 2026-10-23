/*
 * A121069  Conjectured sequence for jumping champions greater than 1 (most
 *          common prime gaps up to x, for some x).
 * https://oeis.org/A121069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 2, a(2) = 4, a(3) = 6, a(n) = product of the first (n-1)
 *            primes for n ≥ 4
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>

static size_t estimate_prime_limit(size_t n)
{
    if (n < 6) return 15;
    double dn = (double)n;
    double est = dn * (log(dn) + log(log(dn)));
    return (size_t)(est + 3.0);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but return 0 */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)6;

    /* We need product of the first (n-1) primes. */
    size_t needed = (size_t)(n - 1);

    /* Prime storage (static across calls). */
    static uint32_t *prime_list = NULL;
    static size_t prime_count = 0;
    static seq_t *prime_prod = NULL;   /* product[i] = product of first (i+1) primes */

    if (prime_count < needed) {
        /* Allocate (or reallocate) prime list up to the required count. */
        size_t limit = estimate_prime_limit(needed);
        uint8_t *sieve = (uint8_t*)malloc(limit + 1);
        if (!sieve) return 0;  /* allocation failure: return 0 */

        for (size_t i = 0; i <= limit; ++i) sieve[i] = 1;
        sieve[0] = sieve[1] = 0;
        for (size_t i = 2; (size_t)i * i <= limit; ++i)
            if (sieve[i])
                for (size_t j = i * i; j <= limit; j += i)
                    sieve[j] = 0;

        /* Count primes up to the limit. */
        size_t count = 0;
        for (size_t i = 2; i <= limit; ++i)
            if (sieve[i]) ++count;

        if (count < needed) {   /* unlikely but handle */
            free(sieve);
            return a(n);        /* try again with larger limit */
        }

        /* Store primes. */
        uint32_t *new_primes = (uint32_t*)malloc(count * sizeof(uint32_t));
        if (!new_primes) { free(sieve); return 0; }

        size_t idx = 0;
        for (size_t i = 2; i <= limit; ++i)
            if (sieve[i]) new_primes[idx++] = (uint32_t)i;

        free(sieve);

        /* Free old storage. */
        free(prime_list);
        free(prime_prod);

        prime_list = new_primes;
        prime_count = count;

        /* Compute products of primes. */
        seq_t *new_prod = (seq_t*)malloc(count * sizeof(seq_t));
        if (!new_prod) { free(prime_list); prime_list = NULL; prime_count = 0; return 0; }

        for (size_t i = 0; i < count; ++i) {
            if (i == 0) new_prod[i] = (seq_t)prime_list[i];
            else new_prod[i] = new_prod[i-1] * (seq_t)prime_list[i];
        }
        prime_prod = new_prod;
    }

    /* For n ≥ 4, a(n) = product of first (n-1) primes = prime_prod[n-2] */
    return prime_prod[needed - 1];
}
