/*
 * A014237  a(n) = (n-th prime) - (n-th nonprime).
 * https://oeis.org/A014237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A000040(n) - A018252(n)
 * Range:     i8 n <= 42, i16 n <= 3954, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = (n-th prime) - (n-th nonprime) */
/* Offset: 1 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Helper buffers (static) */
static seq_t *g_primes = NULL;
static size_t g_prime_cnt = 0;
static seq_t *g_nonprimes = NULL;
static size_t g_nonprime_cnt = 0;
static idx_t g_limit = 200000;
static bool g_initialised = false;

/* Build primes and nonprimes up to g_limit */
static void build_limit(void)
{
    size_t limit = (size_t)g_limit;
    /* Allocate sieve array */
    char *is_prime = (char*)calloc(limit + 1, sizeof(char));
    if (!is_prime)
        return;                     /* Memory allocation failed – give up */
    memset(is_prime + 2, 1, limit - 1); /* 0,1 are non‑prime */

    /* Sieve */
    for (size_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (size_t j = p * p; j <= limit; j += p)
                is_prime[j] = 0;
        }
    }

    /* Count primes */
    size_t prime_est = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++prime_est;

    /* Free old arrays */
    free(g_primes);
    free(g_nonprimes);

    /* Allocate arrays with exact size */
    g_primes = (seq_t*)malloc(prime_est * sizeof(seq_t));
    /* Nonprime count: 1 (the number 1) + composites */
    g_nonprime_cnt = limit - prime_est;
    g_nonprimes = (seq_t*)malloc(g_nonprime_cnt * sizeof(seq_t));

    if (!g_primes || !g_nonprimes) {
        /* Allocation failed – reset to zero */
        free(g_primes);
        free(g_nonprimes);
        g_primes = g_nonprimes = NULL;
        g_prime_cnt = g_nonprime_cnt = 0;
        free(is_prime);
        return;
    }

    /* Fill arrays */
    size_t pc = 0, ncp = 0;
    g_nonprimes[ncp++] = 1;                 /* 1 is the first nonprime */
    for (size_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            g_primes[pc++] = (seq_t)i;
        } else {
            g_nonprimes[ncp++] = (seq_t)i;
        }
    }

    g_prime_cnt = pc;
    /* g_nonprime_cnt already set */

    free(is_prime);
}

static void ensure_prepared(idx_t n)
{
    if (g_initialised && n <= (idx_t)g_prime_cnt && n <= (idx_t)g_nonprime_cnt)
        return;                     /* Already ready */

    /* Increase limit until enough primes/nonprimes exist */
    while (true) {
        build_limit();
        if (!g_initialised) g_initialised = true;

        if (g_prime_cnt >= (size_t)n && g_nonprime_cnt >= (size_t)n)
            break;                  /* Good enough */

        /* Need more numbers – double the limit */
        g_limit *= 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* Undefined for n=0 – return 0 */

    ensure_prepared(n);

    /* Indexing: n is 1‑based, arrays are 0‑based */
    return g_primes[(size_t)(n - 1)] - g_nonprimes[(size_t)(n - 1)];
}
