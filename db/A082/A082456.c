/*
 * A082456  a(n) = prime(n) + prime(n-1) - a(n-1), with a(0)=0, a(1)=127, and
 *          prime(i) is the i-th prime.
 * https://oeis.org/A082456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = prime(n) + prime(n-1) – a(n-1) Initial values: a(0) = 0,
 *            a(1) = 127 prime(i) is the i‑th prime (prime(1)=2)
 * Range:     i8 n <= 2, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

/*------------------------------------------------------------
   Prime storage
  ------------------------------------------------------------*/
static uint32_t *primes      = NULL;   /* primes[0] = prime(1)=2 */
static size_t   prime_cap   = 0;
static size_t   prime_len   = 0;
static uint32_t prime_candidate = 2;   /* next number to test */

/*------------------------------------------------------------
   Sequence cache
  ------------------------------------------------------------*/
static seq_t *seq_cache     = NULL;   /* cached a(n) values */
static size_t seq_cap       = 0;
static size_t seq_len       = 0;       /* number of terms stored */

/*------------------------------------------------------------
   Dynamic array helpers
  ------------------------------------------------------------*/
static void ensure_prime_capacity(size_t needed)
{
    if (prime_cap >= needed) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 128;
    while (new_cap < needed) new_cap *= 2;
    uint32_t *tmp = (uint32_t*)realloc(primes, new_cap * sizeof(uint32_t));
    if (!tmp) abort();
    primes = tmp;
    prime_cap = new_cap;
}

static void ensure_seq_capacity(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_cache = tmp;
    seq_cap = new_cap;
}

/*------------------------------------------------------------
   Prime generation (trial division)
  ------------------------------------------------------------*/
static void generate_next_prime(void)
{
    while (1) {
        int is_prime = 1;
        uint32_t r = (uint32_t)sqrt((double)prime_candidate);
        for (size_t i = 0; i < prime_len; ++i) {
            uint32_t p = primes[i];
            if (p > r) break;
            if (prime_candidate % p == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            ensure_prime_capacity(prime_len + 1);
            primes[prime_len++] = prime_candidate;
            ++prime_candidate;
            return;
        }
        ++prime_candidate;
    }
}

/* Ensure primes[0..n] exist (prime(n+1) as last). */
static void ensure_primes(size_t n)
{
    if (prime_len > n) return;
    if (prime_cap == 0) ensure_prime_capacity(128);
    while (prime_len <= n) generate_next_prime();
}

/*------------------------------------------------------------
   Sequence initialization and computation
  ------------------------------------------------------------*/
static void init_sequence(void)
{
    if (seq_len > 0) return;
    ensure_seq_capacity(64);
    seq_cache[0] = (seq_t)0;          /* a(0) */
    seq_cache[1] = (seq_t)127;        /* a(1) */
    seq_len = 2;
}

static void ensure_sequence(size_t n)
{
    if (seq_len > n) return;
    /* primes up to index n-1 are needed for a(n) */
    ensure_primes(n - 1);
    ensure_seq_capacity(n + 1);
    while (seq_len <= n) {
        size_t i = seq_len;            /* current index to compute */
        seq_t val = (seq_t)primes[i - 1] + (seq_t)primes[i - 2] - seq_cache[i - 1];
        seq_cache[i] = val;
        ++seq_len;
    }
}

/*------------------------------------------------------------
   Public API
  ------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 0) return (seq_t)0;       /* undefined for negative */
    init_sequence();
    ensure_sequence((size_t)n);
    return seq_cache[(size_t)n];
}
