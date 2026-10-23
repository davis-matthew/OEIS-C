/*
 * A138690  Numbers of the form 56+p^2 (where p is a prime).
 * https://oeis.org/A138690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 56 + p_n^2 (p_n = the n-th prime, 1‑based)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static storage for grown prime table */
static uint32_t *primes = NULL;
static size_t prime_count = 0;
static size_t prime_cap = 0;

/* ensure that the table contains at least `need` primes */
static void ensure_primes(size_t need)
{
    if (prime_count >= need)
        return;

    /* initial allocation */
    if (primes == NULL) {
        prime_cap = 16;
        primes = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
        if (!primes) exit(EXIT_FAILURE);
    }

    /* candidate to test for primality */
    uint32_t cand = (prime_count == 0) ? 2 : primes[prime_count - 1] + 1;

    while (prime_count < need) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint32_t p = primes[i];
            uint64_t prod = (uint64_t)p * p;
            if (prod > cand)      /* p^2 is large enough */
                break;
            if (cand % p == 0) {  /* composite */
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_count == prime_cap) {
                prime_cap <<= 1;
                uint32_t *tmp = (uint32_t *)realloc(primes, prime_cap * sizeof(uint32_t));
                if (!tmp) exit(EXIT_FAILURE);
                primes = tmp;
            }
            primes[prime_count++] = cand;
        }
        ++cand;
    }
}

/* public entry point */
extern seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for the requested offset */
        return (seq_t)0;

    size_t idx = (size_t)n;      /* convert unsigned index to size_t */
    ensure_primes(idx);          /* guarantee the prime table is ready */

    seq_t p = (seq_t)primes[idx - 1];   /* p_n */
    return p * p + (seq_t)56;           /* 56 + p_n^2 */
}
