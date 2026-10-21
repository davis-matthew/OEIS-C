/*
 * A066973  a(n) = phi(binomial(2n, n)).
 * https://oeis.org/A066973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(binomial(2*n, n))
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* ---------- Prime sieve for 2*n limits ---------- */
static uint64_t *prime_list = NULL;
static size_t prime_count  = 0;
static size_t prime_limit  = 0;

static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;                           /* already have enough primes */

    size_t new_limit = (size_t)limit;
    if (new_limit < 2)
        new_limit = 2;

    /* Simple Sieve of Eratosthenes */
    unsigned char *sieve = (unsigned char *)calloc(new_limit + 1, 1);
    if (!sieve)                      /* out of memory – nothing we can do */
        return;

    for (size_t i = 2; i <= new_limit; ++i)
        sieve[i] = 1;

    for (size_t i = 2; i * i <= new_limit; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= new_limit; j += i)
                sieve[j] = 0;
        }
    }

    /* Count primes and store them */
    size_t cnt = 0;
    for (size_t i = 2; i <= new_limit; ++i)
        if (sieve[i])
            ++cnt;

    uint64_t *new_primes = (uint64_t *)realloc(prime_list, cnt * sizeof(uint64_t));
    if (!new_primes) {
        free(sieve);
        return;
    }
    prime_list  = new_primes;
    size_t idx = 0;
    for (size_t i = 2; i <= new_limit; ++i)
        if (sieve[i])
            prime_list[idx++] = i;

    prime_count = cnt;
    prime_limit = new_limit;

    free(sieve);
}

/* ---------- Computation of a(n) ---------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for n < 1 */

    idx_t two_n = n << 1;                /* 2*n  (works for uint64_t) */
    ensure_primes(two_n);

    seq_t result = 1;                    /* product of φ‑factors */

    for (size_t i = 0; i < prime_count && prime_list[i] <= two_n; ++i) {
        uint64_t p = prime_list[i];

        /* exponent of p in (2n)!  (Legendre's formula) */
        idx_t val = two_n;
        idx_t exp = 0;
        while (val) {
            val /= p;
            exp += val;
        }

        /* exponent of p in n!   */
        val = n;
        idx_t sub = 0;
        while (val) {
            val /= p;
            sub += val;
        }

        exp -= 2 * sub;                   /* exponent of p in binomial(2n,n) */
        if (exp == 0)
            continue;                    /* p does not divide the binomial */

        /* φ( p^exp ) = (p-1) * p^(exp-1) */
        seq_t term = (seq_t)(p - 1);
        if (exp > 1) {
            seq_t p_pow = 1;
            for (idx_t k = 1; k < exp; ++k)
                p_pow *= (seq_t)p;
            term *= p_pow;
        }
        result *= term;
    }

    return result;
}
