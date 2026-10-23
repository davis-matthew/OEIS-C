/*
 * A166103  Squares of A166104.
 * https://oeis.org/A166103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (b(n))^2 , where b(n) is the nth odd integer * whose
 *            prime factors are all 3 modulo 6 (i.e. 3 or ≡5 (mod 6)). *
 *            a(1)=1, a(2)=9, a(3)=25, ... (OEIS A166103)
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Helper: compute primes up to a fixed limit */
static const idx_t PRIME_LIMIT = 200000;   /* sufficient for n <= 10000 */
static idx_t *g_primes = NULL;
static size_t g_prime_count = 0;

static void init_primes(void)
{
    if (g_primes != NULL)
        return;              /* already initialized */

    /* Sieve of Eratosthenes */
    bool *is_prime = (bool *)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    if (!is_prime)
        return;              /* allocation failed, but build will abort */
    memset(is_prime, true, (PRIME_LIMIT + 1) * sizeof(bool));
    is_prime[0] = is_prime[1] = false;

    for (idx_t p = 2; p * p <= PRIME_LIMIT; ++p)
    {
        if (is_prime[p])
        {
            for (idx_t k = p * p; k <= PRIME_LIMIT; k += p)
                is_prime[k] = false;
        }
    }

    /* Count primes */
    g_prime_count = 0;
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            ++g_prime_count;

    /* Allocate and store primes */
    g_primes = (idx_t *)malloc(g_prime_count * sizeof(idx_t));
    size_t idx = 0;
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            g_primes[idx++] = i;

    free(is_prime);
}

/* Test if n is an allowed number (prime factors all ≡3 or 5 mod 6) */
static bool is_allowed(idx_t n)
{
    if (n == 1)
        return true;

    idx_t temp = n;
    for (size_t i = 0; i < g_prime_count; ++i)
    {
        idx_t p = g_primes[i];
        if (p * p > temp)
            break;

        if (temp % p == 0)
        {
            if (p % 6 == 1)
                return false;      /* forbidden prime factor */
            while (temp % p == 0)
                temp /= p;
        }
    }

    if (temp > 1 && temp % 6 == 1)
        return false;              /* remaining prime factor forbidden */

    return true;
}

/* Cache of previously computed sqrt terms (b(n)) */
static idx_t *g_sqrt_cache = NULL;
static size_t g_cache_size = 0;
static size_t g_cache_cap = 0;
static idx_t g_next_candidate = 1;  /* next odd candidate to test */

/* Ensure that sqrt terms up to the requested index exist */
static void ensure_cache(idx_t n)
{
    if (g_cache_size >= n)
        return;

    init_primes();      /* primes needed for factor tests */

    while (g_cache_size < n)
    {
        if (is_allowed(g_next_candidate))
        {
            if (g_cache_size >= g_cache_cap)
            {
                size_t new_cap = g_cache_cap == 0 ? 16 : g_cache_cap * 2;
                idx_t *tmp = (idx_t *)realloc(g_sqrt_cache, new_cap * sizeof(idx_t));
                if (!tmp)
                    return; /* allocation failed; opaque, but build will abort */
                g_sqrt_cache = tmp;
                g_cache_cap = new_cap;
            }
            g_sqrt_cache[g_cache_size++] = g_next_candidate;
        }
        g_next_candidate += 2;  /* only odd numbers */
    }
}

/* The required sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;     /* undefined for n=0, but return 0 */

    ensure_cache(n);

    seq_t root = (seq_t)g_sqrt_cache[n - 1];
    return root * root;      /* square the sqrt value */
}
