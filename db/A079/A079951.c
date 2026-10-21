/*
 * A079951  Number of primes p with prime(n) == 1 (modulo 2*p).
 * https://oeis.org/A079951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p with prime(n) ≡ 1 (mod 2*p)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------
   Minimal prime sieve and lookup
   ------------------------------------------------------------------ */
static size_t sieve_limit = 0;          /* current sieve upper bound   */
static uint8_t *is_composite = NULL;    /* boolean array for sieve      */
static uint64_t *prime_list = NULL;     /* array of primes (uint64_t)  */
static size_t prime_count = 0;          /* number of primes found      */
static size_t prime_cap = 0;            /* allocated capacity          */

/* Ensure we have at least N primes and all primes <= limit exist.
   For simplicity we build a full sieve up to a fixed large limit
   the first time this function is called. */
static void init_primes(void)
{
    if (sieve_limit != 0) return;  /* already initialized */

    /* Use a generous fixed limit that covers n up to ≈ 200000. */
    sieve_limit = 2000000;          /* 2 · 10⁶  -> prime(200000) ≈ 2 890 277 */
    is_composite = (uint8_t*)malloc((sieve_limit + 1) * sizeof(uint8_t));
    if (!is_composite) exit(EXIT_FAILURE);

    /* Initial capacity for primes vector */
    prime_cap = 200000;
    prime_list = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
    if (!prime_list) exit(EXIT_FAILURE);

    /* Sieve of Eratosthenes */
    for (size_t i = 2; i <= sieve_limit; ++i) {
        if (!is_composite[i]) {
            /* store prime */
            if (prime_count == prime_cap) {
                prime_cap *= 2;
                prime_list = (uint64_t*)realloc(prime_list, prime_cap * sizeof(uint64_t));
                if (!prime_list) exit(EXIT_FAILURE);
            }
            prime_list[prime_count++] = (uint64_t)i;

            /* Mark multiples */
            if (i * i <= sieve_limit) {
                for (size_t j = i * i; j <= sieve_limit; j += i)
                    is_composite[j] = 1;
            }
        }
    }
}

/* Return the nth prime (1‑based). */
static uint64_t nth_prime(idx_t n)
{
    init_primes();
    if ((size_t)n > prime_count)
        exit(EXIT_FAILURE);  /* n too large for our fixed sieve */
    return prime_list[(size_t)n - 1];
}

/* ------------------------------------------------------------------
   Sequence function
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined, but protect */
    init_primes();

    uint64_t prime_n = nth_prime(n);
    uint64_t mod_base = prime_n;
    seq_t count = 0;

    /* Count primes p such that prime_n % (2*p) == 1 */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * 2 > mod_base) break;  /* 2p > prime_n → no more candidates */

        if (prime_n % (2 * p) == 1)
            ++count;
    }
    return count;
}
