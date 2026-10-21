/*
 * A071139  Numbers k such that the sum of distinct primes dividing k is
 *          divisible by the largest prime dividing k.
 * https://oeis.org/A071139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer k ≥ 2 whose sum of distinct prime
 *            factors is divisible by its largest prime factor
 * Range:     u8 n <= 78, u16 n <= 6951, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>                      /* for sqrt; may be inlined in compilers */

static uint32_t *prime_buf = NULL;    /* list of primes */
static size_t   prime_cnt = 0;
static size_t   prime_cap = 0;
static uint32_t prime_limit = 0;      /* largest prime computed so far */

/* Ensure prime_buf contains all primes ≤ limit.  The sieve is redone each
   time a larger limit is requested; for the few thousand terms required
   this is fast enough. */
static void ensure_primes(uint32_t limit)
{
    if (limit <= prime_limit) return;

    bool *sieve = (bool*)calloc(limit + 1U, sizeof(bool));
    if (!sieve) abort();

    for (uint32_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) {
            if (prime_cnt >= prime_cap) {
                size_t new_cap = prime_cap ? prime_cap * 2 : 4;
                uint32_t *tmp = (uint32_t*)realloc(prime_buf, new_cap * sizeof(uint32_t));
                if (!tmp) abort();
                prime_buf = tmp;
                prime_cap = new_cap;
            }
            prime_buf[prime_cnt++] = i;
            if ((uint64_t)i * i <= limit) {
                for (uint32_t j = i * i; j <= limit; j += i)
                    sieve[j] = true;
            }
        }
    }
    free(sieve);
    prime_limit = limit;
}

/* Test if k satisfies the defining property.  Factor k using only
   primes currently in prime_buf; extend that list as necessary. */
static bool good(uint64_t k)
{
    uint64_t sum  = 0;
    uint64_t maxp = 0;
    uint64_t n    = k;

    uint32_t limit = (uint32_t)sqrt((double)n);
    ensure_primes(limit);

    for (size_t idx = 0; idx < prime_cnt; ++idx) {
        uint64_t p = prime_buf[idx];
        if (p > limit || n == 1) break;
        if (n % p == 0) {
            sum += p;
            if (p > maxp) maxp = p;
            while (n % p == 0) n /= p;
            limit = (uint32_t)sqrt((double)n);
        }
    }
    if (n > 1) {                     /* n is now prime */
        sum += n;
        if (n > maxp) maxp = n;
    }
    return maxp != 0 && sum % maxp == 0;
}

/* Dynamic array of found terms */
static uint64_t *seq_arr = NULL;
static size_t   seq_len = 0;
static size_t   seq_cap = 0;
static uint64_t cur_k = 2;           /* next integer to test */

/* Ensure seq_arr has capacity for needed elements. */
static void ensure_seq_cap(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 4;
    while (new_cap < needed) new_cap *= 2;
    uint64_t *tmp = (uint64_t*)realloc(seq_arr, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    seq_arr = tmp;
    seq_cap = new_cap;
}

/* Entry point required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* fallback for offset < 1 */

    size_t target = (size_t)n;

    while (seq_len < target) {
        if (!good(cur_k)) ++cur_k;
        else {
            ensure_seq_cap(seq_len + 1);
            seq_arr[seq_len++] = cur_k;
            ++cur_k;
        }
    }
    return (seq_t)seq_arr[n - 1];
}
