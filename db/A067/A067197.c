/*
 * A067197  Numbers k such that k*(k+1)/2 is not squarefree.
 * https://oeis.org/A067197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 570, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS definition: Numbers k such that k*(k+1)/2 is not squarefree */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Maximum prime for sieving; large enough for n up to ~10000 */
#define SIEVE_MAX_PRIME 200000

static bool primes_initialized = false;
static unsigned int *primes = NULL;
static size_t primes_count = 0;

/* Simple sieve of Eratosthenes to fill 'primes' */
static void init_primes(void)
{
    if (primes_initialized)
        return;
    bool *is_comp = (bool *)calloc(SIEVE_MAX_PRIME + 1, sizeof(bool));
    if (!is_comp)
        return; /* fallback: no primes, but this improbable */

    size_t size = 0;
    for (unsigned int i = 2; i <= SIEVE_MAX_PRIME; ++i) {
        if (!is_comp[i]) {
            if (size == 0) { /* allocate on first prime */
                primes = (unsigned int *)malloc(SIEVE_MAX_PRIME * sizeof(unsigned int));
            }
            primes[size++] = i;
            if ((uint64_t)i * i <= SIEVE_MAX_PRIME) {
                for (unsigned int j = i * i; j <= SIEVE_MAX_PRIME; j += i)
                    is_comp[j] = true;
            }
        }
    }
    primes_count = size;
    free(is_comp);
    primes_initialized = true;
}

/* Return true if k(k+1)/2 has a squared prime factor */
static bool not_squarefree(uint64_t k)
{
    init_primes();
    seq_t t = (seq_t)k * ((seq_t)k + 1) / 2;
    for (size_t i = 0; i < primes_count; ++i) {
        seq_t p2 = (seq_t)primes[i] * (seq_t)primes[i];
        if (p2 > t)
            break;
        if (t % p2 == 0)
            return true;
    }
    return false;
}

/* Memoized sequence generator */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* Undefined for offset 1; return 0 as placeholder */

    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static uint64_t last_k = 0;

    /* Expand cache to at least n terms */
    if (n > len) {
        size_t new_len = n;
        if (new_len > cap) {
            size_t new_cap = (cap == 0) ? 16 : cap * 2;
            while (new_cap < new_len)
                new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cap = new_cap;
        }
        while (len < new_len) {
            ++last_k;
            if (not_squarefree(last_k))
                cache[len++] = (seq_t)last_k;
        }
    }
    return cache[n - 1];
}
