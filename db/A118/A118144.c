/*
 * A118144  Numbers of prime factors of l, where l is defined in A118534.
 * https://oeis.org/A118144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Ω(l) where * l = largest divisor of (2·prime(n) –
 *            prime(n+1)) that is * strictly greater than (prime(n+1) –
 *            prime(n)). * If no such divisor exists, a(n) = 0. * * This
 *            implementation computes primes on demand with a simple * sieve,
 *            finds the divisor, and counts its prime factors
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Global prime list (1‑based indexing via g_primes[0] -> prime(1)=2). */
static uint64_t *g_primes   = NULL;
static size_t   g_prime_cnt = 0;
static size_t   g_prime_cap = 0;

/* Make sure at least 'need' primes are available (need ≥ 1). */
static void ensure_primes(size_t need)
{
    if (g_prime_cnt >= need) return;

    size_t limit = (g_prime_cap > 0 ? g_prime_cap : 2);
    while (true) {
        if (limit < 16) limit = 16;
        char *is_prime = (char *)malloc(limit + 1);
        if (!is_prime) return;                       /* allocation failure */
        for (size_t i = 0; i <= limit; ++i) is_prime[i] = 1;
        is_prime[0] = is_prime[1] = 0;
        for (size_t i = 2; i * i <= limit; ++i) {
            if (is_prime[i]) {
                for (size_t j = i * i; j <= limit; j += i)
                    is_prime[j] = 0;
            }
        }
        /* Count primes */
        size_t cnt = 0;
        for (size_t i = 2; i <= limit; ++i)
            if (is_prime[i]) ++cnt;
        if (cnt >= need) {
            /* Allocate (or grow) primes array */
            if (g_prime_cap < cnt) {
                uint64_t *p = (uint64_t *)realloc(g_primes, cnt * sizeof(uint64_t));
                if (!p) { free(is_prime); return; } /* allocation failure */
                g_primes   = p;
                g_prime_cap = cnt;
            }
            size_t idx = 0;
            for (size_t i = 2; i <= limit; ++i)
                if (is_prime[i]) g_primes[idx++] = (uint64_t)i;
            g_prime_cnt = idx;
            free(is_prime);
            break;
        }
        free(is_prime);
        limit *= 2;                     /* double the limit and try again */
    }
}

/* Count prime factors (with multiplicity) of x. */
static seq_t count_prime_factors(uint64_t x)
{
    seq_t cnt = 0;
    for (size_t i = 0;
         i < g_prime_cnt && (uint64_t)g_primes[i] * g_primes[i] <= x; ++i) {
        uint64_t p = g_primes[i];
        while (x % p == 0) {
            ++cnt;
            x /= p;
        }
    }
    if (x > 1) ++cnt;
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n = 0, but safe */

    /* Need at least n+1 primes to get prime(n) and prime(n+1) */
    ensure_primes((size_t)n + 1);

    uint64_t p_n    = g_primes[(size_t)n - 1];
    uint64_t p_next = g_primes[(size_t)n];
    uint64_t d      = p_next - p_n;      /* prime gap */
    uint64_t S      = 2 * p_n - p_next;  /* equals p_n - d */

    /* Largest divisor of S that is > d */
    uint64_t best = 0;
    for (uint64_t i = 1; i * i <= S; ++i) {
        if (S % i == 0) {
            uint64_t j = S / i;
            if (i > d && i > best) best = i;
            if (j > d && j > best) best = j;
        }
    }

    if (best == 0) return 0;   /* no divisor satisfies the condition */

    return count_prime_factors(best);
}
