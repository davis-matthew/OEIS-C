/*
 * A107623  Primes plus alternately 3 and 2.
 * https://oeis.org/A107623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n) + (2 + mod(n, 2))
 * Range:     u8 n <= 54, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

/* dynamic array of primes */
static idx_t *g_primes = NULL;
static size_t g_primes_size = 0;
static size_t g_primes_cap = 0;
/* simple sieve state */
static unsigned char *g_composite = NULL;
static uint64_t g_sieve_limit = 0;

/* append a prime to the dynamic array */
static void push_prime(idx_t p)
{
    if (g_primes_size == g_primes_cap) {
        size_t new_cap = g_primes_cap ? g_primes_cap * 2 : 128;
        idx_t *tmp = (idx_t *)realloc(g_primes, new_cap * sizeof(idx_t));
        if (!tmp) abort();          /* memory exhaustion */
        g_primes = tmp;
        g_primes_cap = new_cap;
    }
    g_primes[g_primes_size++] = p;
}

/* ensure that the nth prime is available in g_primes[n-1] */
static void ensure_primes(idx_t n)
{
    if (g_primes_size >= n) return;

    /* estimate an upper bound for the nth prime (Rosser's theorem) */
    uint64_t upper;
    if (n < 6) {
        upper = 15;
    } else {
        double dn = (double)n;
        double est = dn * (log(dn) + log(log(dn)));
        upper = (uint64_t)est + 3;   /* safety margin */
    }

    if (upper <= g_sieve_limit) return;   /* already sieved enough */

    if (g_sieve_limit == 0) {
        /* initialise the sieve from scratch */
        g_sieve_limit = upper;
        g_composite = (unsigned char *)calloc(g_sieve_limit + 1, 1);
        if (!g_composite) abort();

        for (uint64_t i = 2; i * i <= g_sieve_limit; ++i) {
            if (!g_composite[i]) {
                for (uint64_t j = i * i; j <= g_sieve_limit; j += i)
                    g_composite[j] = 1;
            }
        }
        for (uint64_t i = 2; i <= g_sieve_limit; ++i)
            if (!g_composite[i]) push_prime((idx_t)i);
    } else {
        /* extend the existing sieve */
        uint64_t old_limit = g_sieve_limit;
        g_sieve_limit = upper;
        g_composite = (unsigned char *)realloc(g_composite,
                                               g_sieve_limit + 1);
        if (!g_composite) abort();
        /* new part is initially zero */
        memset(g_composite + old_limit + 1,
               0, (size_t)(g_sieve_limit - old_limit));

        uint64_t sqrt_limit = (uint64_t)sqrt((double)g_sieve_limit) + 1;
        for (size_t pi = 0; pi < g_primes_size; ++pi) {
            uint64_t p = g_primes[pi];
            if (p > sqrt_limit) break;
            uint64_t start = ((old_limit + 1 + p - 1) / p) * p;
            for (uint64_t j = start; j <= g_sieve_limit; j += p)
                g_composite[j] = 1;
        }
        for (uint64_t i = old_limit + 1; i <= g_sieve_limit; ++i)
            if (!g_composite[i]) push_prime((idx_t)i);
    }
}

static seq_t nth_prime(idx_t n)
{
    ensure_primes(n);
    return (seq_t)g_primes[n - 1];
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* out‑of‑range behaviour is irrelevant */
    seq_t p   = nth_prime(n);
    seq_t add = 2 + ((seq_t)(n & 1));     /* 3 for odd n, 2 for even n */
    return p + add;
}
