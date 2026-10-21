/*
 * A098764  a(n) = 3p - q where p and q are consecutive primes.
 * https://oeis.org/A098764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 3513, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 3*A000040(n) - A000040(n+1)   (A000040 = nth prime)
 *
 * The function generates primes on demand with trial division,
 * storing them in a static array.  At most the first n+1 primes are
 * required, so the generation method easily satisfies the time limit
 * for n <= 10000.
 */
#include <stdint.h>
#include <stdlib.h>

/*--- Prime table (dynamic array) ----------------------------------*/
static uint64_t *prime_list = NULL;  /* primes stored as 64‑bit values   */
static size_t   prime_cnt   = 0;      /* number of primes currently stored */
static size_t   prime_cap   = 0;      /* allocated capacity of prime_list   */

static void enlarge_prime_capacity(size_t needed)
{
    if (prime_cap >= needed)
        return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 64;
    while (new_cap < needed)
        new_cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp) abort();          /* memory allocation failure */
    prime_list = tmp;
    prime_cap  = new_cap;
}

/* Generate primes until we have at least `count` primes in prime_list   */
static void generate_primes_up_to(idx_t count)
{
    if (prime_cnt >= (size_t)count)
        return;

    /* Initialise with the first two primes if the table is empty.      */
    if (prime_cnt == 0) {
        enlarge_prime_capacity(64);
        prime_list[0] = 2;
        prime_list[1] = 3;
        prime_cnt = 2;
    }

    /* Extend the table to the requested size.                           */
    uint64_t candidate = prime_list[prime_cnt - 1] + 2;  /* odd numbers only */
    while (prime_cnt < (size_t)count) {
        int is_prime = 1;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            enlarge_prime_capacity(prime_cnt + 1);
            prime_list[prime_cnt++] = candidate;
        }
        candidate += 2;
    }
}

/*--- Sequence function ------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n < 1; placeholder value */

    /* Ensure primes n and n+1 are available.                       */
    generate_primes_up_to((idx_t)(n + 1));

    seq_t p = prime_list[n - 1];  /* nth prime   */
    seq_t q = prime_list[n];      /* (n+1)th prime */

    return p * 3 - q;
}
