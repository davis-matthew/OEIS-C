/*
 * A097743  Numbers of the form 3*2^(p - 1) - 1 where p is prime.
 * https://oeis.org/A097743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*2^(p(n)-1) - 1 where p(n) is the n‑th prime
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *prime_cache = NULL;   /* buffer holding known primes   */
static size_t prime_cnt   = 0;          /* number of primes stored      */
static size_t prime_cap   = 0;          /* allocated capacity           */

/* enlarge the prime buffer to at least 'needed' elements  */
static void enlarge_prime_cache(size_t needed)
{
    if (prime_cap >= needed)
        return;
    size_t new_cap = prime_cap ? prime_cap << 1 : 16;
    if (new_cap < needed)
        new_cap = needed;
    uint64_t *new_buf = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
    if (!new_buf) { abort(); }               /* failure: abort */
    prime_cache = new_buf;
    prime_cap   = new_cap;
}

/* fill the prime cache so that at least 'n' primes are available */
static void ensure_prime_cache(size_t n)
{
    if (prime_cnt >= n)
        return;
    enlarge_prime_cache(n);
    uint64_t candidate = prime_cnt ? prime_cache[prime_cnt - 1] + 1 : 2;
    if (candidate == 3 && prime_cnt == 0) {   /* ensure 2 is first */
        prime_cache[prime_cnt++] = 2;
    }
    while (prime_cnt < n) {
        int is_prime = 1;
        uint64_t p = 2;
        /* test divisibility by known primes up to sqrt(candidate) */
        for (size_t i = 0; i < prime_cnt; ++i) {
            p = prime_cache[i];
            if (p > candidate / p) break;    /* p*p > candidate   */
            if (candidate % p == 0) { is_prime = 0; break; }
        }
        if (is_prime)
            prime_cache[prime_cnt++] = candidate;
        ++candidate;
    }
}

seq_t a(idx_t n)
{
    /*  n is 1‑based, so the n‑th prime is required  */
    ensure_prime_cache((size_t)n);
    uint64_t p = prime_cache[n-1];    /* the n‑th prime            */

    /* compute 3*2^(p-1) - 1   (exact for any seq_t)   */
    seq_t val = (seq_t)3;
    idx_t shift = (idx_t)(p - 1);
    while (shift--)  val <<= 1;       /* shift by 1 repeatedly  */
    val -= (seq_t)1;

    return val;
}
