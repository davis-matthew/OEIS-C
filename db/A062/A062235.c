/*
 * A062235  a(n) = prime(n)^2 - prime(n+1).
 * https://oeis.org/A062235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n)^2 - prime(n+1)
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/*  Minimal prime table with on‑demand generation using trial division */
/* ------------------------------------------------------------------ */

static uint32_t *g_primes   = NULL;   /* stored primes */
static size_t   g_prime_count = 0;    /* number of primes stored */
static size_t   g_prime_cap   = 0;    /* allocated capacity */
static uint32_t g_next_candidate = 2; /* next integer to test for primality */

/* Grow the prime table to contain at least 'need' primes (need >= 1). */
static void ensure_primes_count(size_t need)
{
    while (g_prime_count < need) {
        uint32_t cand = g_next_candidate;
        for (;;) {
            int is_prime = 1;
            for (size_t i = 0; i < g_prime_count; ++i) {
                uint32_t p = g_primes[i];
                if ((uint64_t)p * (uint64_t)p > (uint64_t)cand)
                    break;
                if (cand % p == 0) {
                    is_prime = 0;
                    break;
                }
            }
            if (is_prime) {
                /* Ensure enough capacity. */
                if (g_prime_count == g_prime_cap) {
                    size_t new_cap = g_prime_cap == 0 ? 16 : g_prime_cap << 1;
                    uint32_t *tmp = (uint32_t *)realloc(g_primes, new_cap * sizeof(uint32_t));
                    if (!tmp) {
                        /* allocation failed – abort (undefined behaviour). */
                        abort();
                    }
                    g_primes   = tmp;
                    g_prime_cap = new_cap;
                }
                g_primes[g_prime_count++] = cand;
                g_next_candidate = cand + 1;
                break;
            }
            ++cand;   /* try next odd/even number */
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Sequence function                                                */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined for offset < 1 */
    /* We need primes up to index n+1 to compute prime(n+1). */
    ensure_primes_count((size_t)n + 1);

    seq_t p_n     = (seq_t)g_primes[n - 1];   /* nth prime, 1‑based */
    seq_t p_next  = (seq_t)g_primes[n];       /* (n+1)‑th prime     */

    return p_n * p_n - p_next;
}
