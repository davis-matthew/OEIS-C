/*
 * A073800  Remainder of division 2^n/c(n), where c(n)=A002808(n), the n-th
 *          composite.
 * https://oeis.org/A073800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n mod c(n), where c(n) is the n‑th composite number
 *            (OEIS A002808)
 * Range:     u8 n <= 235, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Prime / composite lists (grow dynamically) */
static uint32_t *primes = NULL;
static size_t primes_used = 0, primes_cap = 0;
static uint32_t *composites = NULL;
static size_t composites_used = 0, composites_cap = 0;
static uint32_t last_checked = 4;        /* next integer to test for primeness */

/* Ensure capacity of the primes array */
static void ensure_primes_cap(size_t needed)
{
    if (needed <= primes_cap) return;
    size_t new_cap = primes_cap ? primes_cap * 2 : 128;
    if (new_cap < needed) new_cap = needed;
    uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
    primes = tmp;
    primes_cap = new_cap;
}

/* Ensure capacity of the composites array */
static void ensure_composites_cap(size_t needed)
{
    if (needed <= composites_cap) return;
    size_t new_cap = composites_cap ? composites_cap * 2 : 256;
    if (new_cap < needed) new_cap = needed;
    uint32_t *tmp = (uint32_t *)realloc(composites, new_cap * sizeof(uint32_t));
    composites = tmp;
    composites_cap = new_cap;
}

/* Add a prime to the list */
static void add_prime(uint32_t v)
{
    ensure_primes_cap(primes_used + 1);
    primes[primes_used++] = v;
}

/* Add a composite to the list */
static void add_composite(uint32_t v)
{
    ensure_composites_cap(composites_used + 1);
    composites[composites_used++] = v;
}

/* Simple primality test using the primes already discovered */
static int is_prime_candidate(uint32_t v)
{
    if (v < 2) return 0;
    for (size_t i = 0; i < primes_used; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > v) break;
        if (v % p == 0) return 0;
    }
    return 1;
}

/* Make sure the primes list contains at least the primes needed
   for testing composites up to the current limit. */
static void init_prime_list(void)
{
    if (primes_used == 0) {
        add_prime(2);
        add_prime(3);
    }
}

/* Ensure that composites[] has at least 'n' (>=1) composites stored */
static void ensure_composites_count(idx_t n)
{
    init_prime_list();
    while (composites_used < (size_t)n) {
        uint32_t cand = last_checked++;
        if (is_prime_candidate(cand))
            add_prime(cand);
        else
            add_composite(cand);
    }
}

/* Modular exponentiation: (base^exp) mod mod, works for all seq_t types */
static seq_t pow_mod(seq_t base, idx_t exp, seq_t mod)
{
    seq_t result = (seq_t)1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ull)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

/* Sequence function: a(n) = 2^n mod c(n) */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for offset‑zero indices */
    ensure_composites_count(n);          /* make sure we have the n‑th composite */
    seq_t comp = (seq_t)composites[n - 1];
    return pow_mod((seq_t)2, n, comp);
}
