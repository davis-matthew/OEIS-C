/*
 * A230124  Squarefree evil numbers.
 * https://oeis.org/A230124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Squarefree evil numbers: squarefree integers whose binary representation
   contains an even number of 1‑bits. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---- Parameters ---- */
#define MAX_TERMS 20000          /* enough for n up to about 10 000 */
#define PRIME_LIMIT 200000       /* enough to test squarefreeness of the
                                   numbers we will encounter */

/* ---- State ---- */
static seq_t        cache[MAX_TERMS];
static idx_t       cache_len  = 0;
static idx_t       next_int   = 2;          /* next integer to test */

static uint64_t     primes[PRIME_LIMIT];
static size_t      primes_cnt = 0;
static bool        primes_built = false;

/* ---- Helpers ---- */

/* Build a list of primes up to PRIME_LIMIT using a simple sieve. */
static void build_primes(void)
{
    if (primes_built) return;
    static bool is_prime[PRIME_LIMIT + 1];
    for (size_t i = 0; i <= PRIME_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = false;
        }
    }
    primes_cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) primes[primes_cnt++] = (uint64_t)i;
    primes_built = true;
}

/* Test whether n is squarefree using the precomputed primes. */
static bool is_squarefree(idx_t n)
{
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > n / p) break;          /* p*p > n */
        if (n % (p * p) == 0) return false;
    }
    return true;
}

/* Test whether n is an evil number (even popcount). */
static bool is_evil(idx_t n)
{
    int parity = 0;   /* 0 = even number of 1-bits so far */
    while (n) {
        parity ^= 1;
        n &= n - 1;   /* drop lowest set bit */
    }
    return parity == 0;
}

/* Ensure that cache contains at least 'required' terms. */
static void ensure_cache(idx_t required)
{
    if (required <= cache_len) return;

    build_primes();  /* make primes available */

    while (cache_len < required) {
        if (cache_len >= MAX_TERMS) {
            /* We asked for more terms than we can store.
               In practice this should not happen for the intended limits. */
            abort();
        }
        if (is_squarefree(next_int) && is_evil(next_int)) {
            cache[cache_len++] = (seq_t)next_int;
        }
        ++next_int;
    }
}

/* ---- Public API ---- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined; return 0 safely */
    ensure_cache(n);
    return cache[n - 1];
}
