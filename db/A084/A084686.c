/*
 * A084686  Take n-th prime p(n), rewrite it with digits in decreasing order
 *          to get b(n), then a(n)=(b(n)-p(n))/9.
 * https://oeis.org/A084686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base, look
 * Formula:   a(n) = (b(n) - p(n)) / 9 * where p(n) is the n‑th prime and b(n)
 *            is p(n) with its decimal * digits sorted in decreasing order
 * Range:     u8 n <= 27, u16 n <= 1491, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* We keep a static array of primes and generate them on demand.
 * 200,000 primes are enough for all tests (the 10000‑th prime is 104729). */
#define MAX_PRIMES 200000U
static idx_t prime_cache[MAX_PRIMES];
static size_t prime_cache_cnt = 0;
static idx_t next_candidate = 3;

/* Populate prime_cache until it contains at least n primes.
 * The routine uses simple trial division with the primes already found. */
static void ensure_primes(idx_t n)
{
    if (prime_cache_cnt >= n)
        return;

    /* Initialise the cache with the first prime. */
    if (prime_cache_cnt == 0) {
        prime_cache[prime_cache_cnt++] = 2;
        next_candidate = 3;
    }

    while (prime_cache_cnt < n) {
        idx_t x = next_candidate;
        bool is_prime = true;
        for (size_t i = 0; i < prime_cache_cnt; ++i) {
            idx_t p = prime_cache[i];
            if (p * p > x)
                break;
            if (x % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_cache_cnt < MAX_PRIMES)
                prime_cache[prime_cache_cnt++] = x;
        }
        next_candidate += 2;  /* skip even numbers */
    }
}

/* Compute A084686(n).  n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0)      /* Undefined, but return 0 to keep the function defined */
        return (seq_t)0;

    /* Make sure the n‑th prime is available. */
    ensure_primes(n);

    /* Retrieve the n‑th prime. */
    idx_t prime_val = prime_cache[n - 1];

    /* Count the digits of the prime (we can do this with idx_t arithmetic). */
    unsigned int freq[10] = {0};
    idx_t tmp = prime_val;
    while (tmp > 0) {
        unsigned int d = (unsigned int)(tmp % 10);
        freq[d]++;
        tmp /= 10;
    }

    /* Form the number with digits in descending order using seq_t. */
    seq_t b = 0;
    for (int d = 9; d >= 0; --d) {
        for (unsigned int k = 0; k < freq[d]; ++k) {
            b = b * ((seq_t)10) + (seq_t)d;
        }
    }

    /* Compute (b - p) / 9.  The division is exact. */
    seq_t diff = b - (seq_t)prime_val;
    seq_t result = diff / ((seq_t)9);
    return result;
}
