/*
 * A127333  Numbers that are the sum of 6 consecutive primes.
 * https://oeis.org/A127333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=0}^{5} prime(n+i)
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms of OEIS sequence A127333: sum of six consecutive primes. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static cache of generated primes. */
static seq_t *primes = NULL;       /* array of primes */
static size_t prime_cnt = 0;       /* number of primes stored */
static size_t prime_cap = 0;       /* allocated capacity */
static seq_t last_checked = 2;     /* last number tested for primality */

/* Extend the prime cache until at least 'needed' primes are available. */
static void extend_primes_to(idx_t needed)
{
    if (needed <= prime_cnt)
        return;

    /* Initialise cache on first call. */
    if (primes == NULL) {
        prime_cap = 64;
        primes = (seq_t *)malloc(prime_cap * sizeof(seq_t));
        if (primes == NULL)
            return;               /* allocation failed – unlikely in tests */
        primes[0] = (seq_t)2;
        prime_cnt = 1;
        last_checked = (seq_t)2;
    }

    seq_t candidate = last_checked + 1;
    while (prime_cnt < needed) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            seq_t p = primes[i];
            /* Check p*p > candidate without overflow: if candidate/p < p then p*p > candidate */
            if (candidate / p < p)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                seq_t *new_arr = (seq_t *)realloc(primes, prime_cap * sizeof(seq_t));
                if (new_arr == NULL)
                    return;           /* allocation failed – leave cache partial */
                primes = new_arr;
            }
            primes[prime_cnt++] = candidate;
        }
        ++candidate;
    }
    last_checked = candidate - 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;            /* undefined for n < 1; returns 0 */

    idx_t required_index = n + 5;    /* need primes up to index n+5 (1‑based) */
    extend_primes_to(required_index);

    seq_t sum = (seq_t)0;
    size_t start = (size_t)(n - 1);   /* convert 1‑based index to 0‑based array index */
    for (idx_t i = 0; i < 6; ++i) {
        sum += primes[start + i];
    }
    return sum;
}
