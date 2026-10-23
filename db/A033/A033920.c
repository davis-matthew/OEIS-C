/*
 * A033920  Largest proper substring of n-th prime that is prime (0 if none).
 * https://oeis.org/A033920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Sequence: Largest proper prime substring of the n-th prime.
   a(n) is the greatest prime that appears as a proper, contiguous
   substring of the decimal representation of the n‑th prime.
   If no such prime exists, a(n) = 0.  */
   
/* --- static cache for generated primes ------------------------------------------ */
static uint64_t *primes_list  = NULL;   /* primes in increasing order */
static size_t   primes_cnt    = 0;      /* number of primes stored */
static uint64_t sieve_limit   = 0;      /* highest prime already sieved */

/* ----------------------------------------------------------------------------- */
static void ensure_primes_upto(uint64_t limit)
{
    if (limit <= sieve_limit)
        return;                       /* already enough */

    /* Sieve of Eratosthenes for [2, limit] */
    char *is_comp = (char*)calloc((size_t)limit + 1, 1);
    if (!is_comp)
        abort();                       /* memory exhausted */

    for (uint64_t i = 2; i * i <= limit; ++i)
        if (!is_comp[i])
            for (uint64_t j = i * i; j <= limit; j += i)
                is_comp[j] = 1;

    /* count primes */
    size_t new_cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!is_comp[i])
            ++new_cnt;

    uint64_t *new_primes = (uint64_t*)malloc(new_cnt * sizeof(uint64_t));
    if (!new_primes)
        abort();

    size_t pos = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!is_comp[i])
            new_primes[pos++] = i;

    free(is_comp);
    free(primes_list);

    primes_list = new_primes;
    primes_cnt  = new_cnt;
    sieve_limit = limit;
}

/* ----------------------------------------------------------------------------- */
static bool is_prime_small_uint64(uint64_t x)
{
    if (x < 2)
        return false;
    uint64_t root = (uint64_t)(sqrt((double)x) + 1e-9);
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes_list[i];
        if (p > root)
            break;
        if (x % p == 0)
            return false;
    }
    return true;
}

/* ----------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                       /* out of bounds, spec assumes n>=1 */
        return 0;

    /* Estimate an upper bound for the n‑th prime.
       For n >= 6, p_n < n (log n + log log n).            */
    uint64_t limit;
    if (n < 6)
        limit = 15;
    else
        limit = (uint64_t)(n * (log((double)n) + log(log((double)n))) + 10);

    ensure_primes_upto(limit);

    /* Retrieve the n‑th prime (primes_list is zero‑based). */
    uint64_t prime = primes_list[n - 1];

    /* Convert prime to decimal digits (most significant first). */
    uint8_t digits[64];
    size_t len = 0;
    uint64_t tmp = prime;
    do {
        digits[len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    } while (tmp > 0);

    /* Reverse to obtain correct order. */
    for (size_t i = 0; i < len / 2; ++i) {
        uint8_t t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    /* Scan all proper substrings and keep the largest prime. */
    seq_t best = 0;
    for (size_t l = 1; l < len; ++l)                    /* substring length */
        for (size_t start = 0; start + l <= len; ++start) {
            uint64_t val = 0;
            for (size_t k = start; k < start + l; ++k)
                val = val * 10 + digits[k];

            if (val > 1 && is_prime_small_uint64(val)) {
                if ((seq_t)val > best)
                    best = (seq_t)val;
            }
        }

    return best;
}
