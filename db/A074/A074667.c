/*
 * A074667  Seven-digit distinct-digit primes.
 * https://oeis.org/A074667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 9614, u64 n <= 9614, u128 n <= 9614,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A074667: Seven‑digit distinct‑digit primes
   a(n) = the n‑th prime p with 1 000 000 ≤ p ≤ 9 999 999
          and all seven decimal digits distinct.  */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static const idx_t MAX_SIEVE = 10000000UL; /* prime sieve up to 10^7 */

/* static state for lazy initialization */
static bool sieve_done = false;
static bool cache_done = false;
static bool *is_composite = NULL;          /* sieve */
static seq_t *prime_cache = NULL;          /* cache of distinct‑digit primes */
static idx_t cache_size = 0;               /* number of primes stored */
static idx_t cache_cap  = 0;               /* capacity of prime_cache */

/* test if a 7‑digit number has all distinct decimal digits */
static bool distinct_digits(idx_t n)
{
    unsigned char seen[10] = {0};
    for (int i = 0; i < 7; ++i) {
        unsigned d = n % 10;
        n /= 10;
        if (seen[d]) return false;
        seen[d] = 1;
    }
    return true;
}

/* simple Eratosthenes sieve up to MAX_SIEVE */
static void build_sieve(void)
{
    if (sieve_done) return;
    is_composite = (bool*)malloc((MAX_SIEVE + 1) * sizeof(bool));
    if (!is_composite) return;
    memset(is_composite, 0, (MAX_SIEVE + 1) * sizeof(bool));
    is_composite[0] = is_composite[1] = 1;
    for (idx_t i = 2; i * i <= MAX_SIEVE; ++i) {
        if (!is_composite[i]) {
            for (idx_t j = i * i; j <= MAX_SIEVE; j += i)
                is_composite[j] = 1;
        }
    }
    sieve_done = true;
}

/* pre‑compute all 7‑digit distinct‑digit primes into prime_cache */
static void build_cache(void)
{
    if (cache_done) return;
    /* initial capacity guess: 35000 primes */
    cache_cap = 35000;
    prime_cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    if (!prime_cache) return;
    idx_t count = 0;
    for (idx_t n = 1000000; n <= 9999999; ++n) {
        if (!is_composite[n] && distinct_digits(n)) {
            if (count == cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t*)realloc(prime_cache, cache_cap * sizeof(seq_t));
                if (!tmp) break;
                prime_cache = tmp;
            }
            prime_cache[count++] = (seq_t)n;
        }
    }
    cache_size = count;
    cache_done = true;
}

/* public access function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 */
    /* ensure sieve and cache are built */
    if (!sieve_done) build_sieve();
    if (!cache_done) {
        build_cache();
        /* If cache still empty (unlikely), ensure sieve_done to avoid loops */
        if (!cache_done) return 0;
    }
    if (n > cache_size) return 0;   /* out of range */
    return prime_cache[n - 1];
}
