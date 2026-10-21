/*
 * A066652  Primes of the form 2*s - 1, where s is a squarefree number
 *          (A005117).
 * https://oeis.org/A066652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 3659, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the n‑th prime p for which (p+1)/2 is squarefree. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static idx_t *primes = NULL;
static size_t primes_len = 0, primes_cap = 0;
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static idx_t cur = 3;

static void ensure_primes_cap(size_t need)
{
    if (primes_cap < need) {
        size_t newcap = primes_cap ? primes_cap * 2 : 8;
        if (newcap < need) newcap = need;
        idx_t *tmp = (idx_t *)realloc(primes, newcap * sizeof(idx_t));
        if (!tmp) exit(1);
        primes = tmp;
        primes_cap = newcap;
    }
}

static void add_prime(idx_t p)
{
    ensure_primes_cap(primes_len + 1);
    primes[primes_len++] = p;
}

static void ensure_cache_cap(size_t need)
{
    if (cache_cap < need) {
        size_t newcap = cache_cap ? cache_cap * 2 : 8;
        if (newcap < need) newcap = need;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = newcap;
    }
}

static void add_cache(seq_t v)
{
    ensure_cache_cap(cache_len + 1);
    cache[cache_len++] = v;
}

static bool is_squarefree(seq_t v)
{
    seq_t x = v;
    for (size_t i = 0; i < primes_len; ++i) {
        seq_t p = (seq_t)primes[i];
        if (p * p > x) break;
        int cnt = 0;
        while (x % p == 0) {
            x /= p;
            ++cnt;
            if (cnt > 1) return false;
        }
        if (x == 1) break;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* offset is 1 but guarded */

    if (primes == NULL) {         /* one‑time initialization */
        primes_cap = 8;
        primes = (idx_t *)malloc(primes_cap * sizeof(idx_t));
        primes_len = 0;
        add_prime(2);             /* include 2 for factor tests */
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    while (cache_len < n) {
        /* test cur for primality */
        bool isprime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            seq_t p = (seq_t)primes[i];
            if (p * p > cur) break;
            if (cur % primes[i] == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            add_prime(cur);
            seq_t s = (cur + 1) / 2;
            if (is_squarefree(s))
                add_cache(cur);
        }
        cur += 2;
    }

    return cache[n - 1];
}
