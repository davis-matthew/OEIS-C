/*
 * A069565  a(0) = 1, a(n) = k*a(n-1) + 1 is a multiple of n-th prime. If no
 *          such number exists then a(n) = 0 and a(n+1) = k*a(n-1) + 1 is a
 *          multiple of (n+1)-th prime; i.e., a(r) = smallest multiple of the
 *          r-th prime = k* a(s) + 1 where a(s) is the last nonzero term.
 * https://oeis.org/A069565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Definition: a(0) = 1.
 * For n ≥ 1 let p be the n‑th prime.  a(n) is the smallest number of
 * the form k·a(n‑1)+1 that is divisible by p.  If no such k exists,
 * then a(n) = 0 and the same a(n‑1) value is used for the next prime.
 * This implementation memoises values and recomputes them on demand.
 */

#include <stdint.h>
#include <stdlib.h>

/* Helper: modular inverse of a modulo m (m is prime, a≠0 mod m).        */
static uint64_t modinv_uint64(uint64_t a, uint64_t m)
{
    int64_t t = 0, newt = 1;
    int64_t r = (int64_t)m, newr = (int64_t)a;
    while (newr != 0) {
        int64_t q = r / newr;
        int64_t tmp = t - q * newt; t = newt; newt = tmp;
        tmp = r - q * newr; r = newr; newr = tmp;
    }
    if (r > 1) return 0;               /* not invertible (should not happen) */
    if (t < 0) t += m;
    return (uint64_t)t;
}

/* Prime table: primes[0] = 2, primes[1] = 3, … up to around 2 000 000.  */
static unsigned primes[200000];
static size_t num_primes = 0;

/* Generate all primes up to LIMIT using a simple sieve. */
static void init_primes(void)
{
    if (num_primes > 0) return;             /* already initialised */
    const size_t LIMIT = 200000;             /* primes up to 200 000      */
    char *sieve = (char*)malloc(LIMIT + 1);
    for (size_t i = 0; i <= LIMIT; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;
    for (size_t i = 2; i * i <= LIMIT; ++i)
        if (sieve[i])
            for (size_t j = i * i; j <= LIMIT; j += i)
                sieve[j] = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (sieve[i]) primes[num_primes++] = (unsigned)i;
    free(sieve);
}

/* Cache for computed a(n) values. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t computed = 0;        /* number of entries actually computed */
static seq_t last_nonzero = 0;    /* last a(m) that was non‑zero          */

/* Ensure the cache is large enough. */
static void ensure_cache(idx_t n)
{
    if ((idx_t)cache_cap > n) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while ((idx_t)new_cap <= n) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    /* One‑time initialisation. */
    static int initialized = 0;
    if (!initialized) {
        init_primes();
        ensure_cache(0);
        cache[0] = (seq_t)1;
        last_nonzero = (seq_t)1;
        computed = 1;
        initialized = 1;
    }

    /* Return cached value if already computed. */
    if (n < computed) return cache[n];

    ensure_cache(n);

    for (idx_t i = computed; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
            last_nonzero = (seq_t)1;
        } else {
            unsigned p = primes[i - 1];           /* n‑th prime */
            seq_t modval = last_nonzero % (seq_t)p;
            /* If a(n-1) is divisible by p, no k exists. */
            if (modval == (seq_t)0) {
                cache[i] = (seq_t)0;
                /* last_nonzero stays unchanged */
            } else {
                uint64_t last_mod = (uint64_t)modval;
                uint64_t inv = modinv_uint64(last_mod, (uint64_t)p);
                uint64_t k = (p - inv % p) % p;
                if (k == 0) k = p;               /* smallest positive k */
                seq_t val = (seq_t)k * last_nonzero + (seq_t)1;
                cache[i] = val;
                last_nonzero = val;
            }
        }
    }

    computed = n + 1;
    return cache[n];
}
