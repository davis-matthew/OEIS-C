/*
 * A095874  a(n) = k if n = A000961(k) (powers of primes), a(n) = 0 if n is
 *          not in A000961.
 * https://oeis.org/A095874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (is_prime_power(n) ? count_prime_powers(n) : 0)
 * Range:     u8 n <= 1432, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A095874:  a(n) = k if n is the k‑th power of a prime,
   otherwise a(n) = 0.  The implementation builds the cumulative
   count of prime powers up to each index and returns that count
   only when the index itself is a prime power.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* --- global state for small prime list --- */
static uint64_t *g_primes = NULL;
static size_t     g_prime_cnt = 0;
static idx_t      g_prime_limit = 0;

/* --- global state for cumulative count memoisation --- */
static seq_t *g_cum = NULL;
static idx_t  g_cum_size = 0;

/* Ensure g_primes contains all primes <= 'limit'. */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= g_prime_limit) return;

    idx_t n = limit;
    bool *is_comp = (bool *)calloc((size_t)n + 1, sizeof(bool));
    if (!is_comp) return;                /* allocation failure – give up */
    is_comp[0] = is_comp[1] = true;
    for (idx_t i = 2; i * i <= n; ++i)
        if (!is_comp[i])
            for (idx_t j = i * i; j <= n; j += i)
                is_comp[j] = true;

    size_t cnt = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (!is_comp[i]) ++cnt;

    uint64_t *pr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!pr) { free(is_comp); return; }

    size_t k = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (!is_comp[i]) pr[k++] = i;

    free(is_comp);
    free(g_primes);
    g_primes = pr;
    g_prime_cnt = cnt;
    g_prime_limit = n;
}

/* Test whether n is a power of a single prime (including 1). */
static bool is_prime_power(idx_t n)
{
    if (n == 0) return false;
    if (n == 1) return true;          /* 1 is considered a prime power */

    idx_t  tmp = n;
    idx_t  distinct = 0;
    idx_t  sq = (idx_t)floor(sqrt((long double)tmp));
    ensure_primes_upto(sq);

    for (size_t i = 0; i < g_prime_cnt && g_primes[i] <= sq; ++i) {
        uint64_t p = g_primes[i];
        if (tmp % p == 0) {
            ++distinct;
            while (tmp % p == 0)
                tmp /= p;
            if (distinct > 1) return false;   /* more than one prime factor */
        }
    }
    if (tmp > 1) ++distinct; /* leftover prime factor > sqrt(n) */
    return distinct == 1;
}

/* Extend the memoised cumulative count array up to 'limit'. */
static void extend_cum_to(idx_t limit)
{
    if (limit <= g_cum_size) return;

    idx_t new_size = limit + 1;                   /* we need indices 0..limit */
    seq_t *new_mem = (seq_t *)realloc(g_cum, new_size * sizeof(seq_t));
    if (!new_mem) return;                        /* allocation failure – give up */
    g_cum = new_mem;

    if (g_cum_size == 0) g_cum[0] = 0;            /* base case for index 1 */

    for (idx_t i = g_cum_size + 1; i <= limit; ++i) {
        if (is_prime_power(i))
            g_cum[i] = g_cum[i - 1] + 1;
        else
            g_cum[i] = g_cum[i - 1];
    }
    g_cum_size = limit;
}

/* Public entry point – returns a(n). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                         /* offset starts at 1 */
    extend_cum_to(n);
    if (is_prime_power(n))
        return g_cum[n];
    else
        return 0;
}
