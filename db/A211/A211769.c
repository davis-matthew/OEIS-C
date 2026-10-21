/*
 * A211769  Greater of twin primes p, p+2 with unique values of sigma(p) and
 *          sigma(p+2); sigma(n) = A000203(n) = sum of divisors of n.
 * https://oeis.org/A211769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 240, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by checking twin primes p,p+2 up to a sieve limit and selecting
   those where sigma(p) and sigma(p+2) occur exactly once among all numbers
   up to the limit. The returned value is the larger twin prime. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>   /* for size_t */

static bool initialized = false;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;
static seq_t *seq      = NULL;

/* Append a value to the result array, expanding it if necessary. */
static void push_result(seq_t val)
{
    if (seq_len == seq_cap) {
        seq_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
    }
    seq[seq_len++] = val;
}

/* One‑time initialization that precomputes the sequence up to a
   safely large sieve limit. */
static void init_sequence(idx_t /*n*/)
{
    if (initialized) return;

    /* Choose a limit that is (conservatively) large enough for 10 000 terms.
       20 000 000 is sufficient for the known size of the sequence. */
    const idx_t LIMIT = 20000000ULL;

    /* --- 1. Smallest prime factor array (spf). --- */
    uint32_t *spf = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    for (idx_t i = 0; i <= LIMIT; ++i) spf[i] = (uint32_t)i;

    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (spf[i] == (uint32_t)i) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
        }
    }

    /* --- 2. Compute sigma(n) for 2..LIMIT. --- */
    uint32_t *sigma = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    sigma[1] = 1;
    uint32_t max_sigma = 1;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        uint64_t s = 1;
        uint32_t x = (uint32_t)i;
        while (x > 1) {
            uint32_t p = spf[x];
            uint64_t pp = 1;
            while (x % p == 0) {
                x /= p;
                pp *= p;
            }
            /* sigma(p^e) = (p^(e+1)-1)/(p-1) */
            uint64_t term = (pp * p - 1) / (p - 1);
            s *= term;
        }
        sigma[i] = (uint32_t)s;
        if (sigma[i] > max_sigma) max_sigma = sigma[i];
    }

    /* --- 3. Frequency count of sigma values (only 1 and 2 are needed). --- */
    uint8_t *freq = (uint8_t *)calloc(max_sigma + 1, sizeof(uint8_t));
    for (idx_t i = 2; i <= LIMIT; ++i) {
        uint32_t v = sigma[i];
        if (freq[v] < 2) freq[v] += 1;
    }

    /* --- 4. Prime sieve for twin prime detection. --- */
    uint8_t *is_prime = (uint8_t *)malloc((LIMIT + 1) * sizeof(uint8_t));
    for (idx_t i = 0; i <= LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* --- 5. Collect valid twin primes. --- */
    for (idx_t p = 3; p + 2 <= LIMIT; ++p) {
        if (is_prime[p] && is_prime[p + 2]) {
            uint32_t s1 = sigma[p];
            uint32_t s2 = sigma[p + 2];
            if (freq[s1] == 1 && freq[s2] == 1) {
                push_result((seq_t)(p + 2));
            }
        }
    }

    /* Free temporary arrays. */
    free(spf);
    free(sigma);
    free(freq);
    free(is_prime);

    initialized = true;
}

/* Public entry point as required. */
seq_t a(idx_t n)
{
    init_sequence(n);
    if (n == 0 || n > seq_len) return 0;   /* safety guard for out of bounds */
    return seq[n - 1];
}
