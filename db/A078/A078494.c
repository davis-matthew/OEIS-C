/*
 * A078494  Primes occurring only once in their decade.
 * https://oeis.org/A078494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 2938, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth prime that is the only prime in its decade.
   The routine generates primes sequentially, keeps track of the prime
   count in the current decade, and records a prime as a term of the
   sequence only when the following decade contains a different prime. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint32_t *g_primes     = NULL;
static size_t   g_primes_len  = 0;
static size_t   g_primes_cap  = 0;

static uint32_t *g_iso        = NULL;
static size_t   g_iso_len     = 0;
static size_t   g_iso_cap     = 0;

static uint32_t g_current_decade = 0;
static uint32_t g_decade_count   = 0;
static uint32_t g_last_prime_in_decade = 0;
static uint32_t g_last_checked = 0;
static bool     g_initialized  = false;

static void add_prime(uint32_t p)
{
    if (g_primes_len == g_primes_cap) {
        size_t newcap = g_primes_cap ? g_primes_cap * 2 : 8;
        uint32_t *tmp = (uint32_t *)realloc(g_primes, newcap * sizeof(uint32_t));
        g_primes = tmp;
        g_primes_cap = newcap;
    }
    g_primes[g_primes_len++] = p;
}

static void add_iso(uint32_t p)
{
    if (g_iso_len == g_iso_cap) {
        size_t newcap = g_iso_cap ? g_iso_cap * 2 : 8;
        uint32_t *tmp = (uint32_t *)realloc(g_iso, newcap * sizeof(uint32_t));
        g_iso = tmp;
        g_iso_cap = newcap;
    }
    g_iso[g_iso_len++] = p;
}

static bool is_prime(uint32_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < g_primes_len; ++i) {
        uint32_t p = g_primes[i];
        if ((uint64_t)p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

static uint32_t next_prime(uint32_t start)
{
    for (uint32_t n = start + 1;; ++n) {
        if (is_prime(n)) {
            add_prime(n);
            return n;
        }
    }
}

static void ensure(idx_t target)
{
    while (g_iso_len < (size_t)target) {
        uint32_t p = next_prime(g_last_checked);
        g_last_checked = p;
        uint32_t decade = p / 10;

        if (!g_initialized) {
            g_initialized = true;
            g_current_decade = decade;
            g_decade_count   = 1;
            g_last_prime_in_decade = p;
            continue;
        }

        if (decade == g_current_decade) {
            ++g_decade_count;
            g_last_prime_in_decade = p;
        } else {
            if (g_decade_count == 1) {
                add_iso(g_last_prime_in_decade);
            }
            g_current_decade = decade;
            g_decade_count   = 1;
            g_last_prime_in_decade = p;
        }
    }
}

seq_t a(idx_t n)
{
    ensure(n);
    return (seq_t)g_iso[(size_t)n - 1];
}
