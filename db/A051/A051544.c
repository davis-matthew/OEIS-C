/*
 * A051544  Quotients of consecutive values of lcm {1, 4, 7, 10, 13
 *          ...,(3n+1)} (A016777).
 * https://oeis.org/A051544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* A051544(n) = L(n) / L(n-1),
 *   where L(n) = lcm{ 1,4,7,...,(3n+1) }.
 * The factorisation of (3n+1) is enough to obtain
 *   a(n) = (3n+1) / gcd(L(n-1), 3n+1).  */

/* Sieve upper bound.  For n ≤ 10000 we need numbers ≤ 30001;
 * we take a generous bound to allow larger n if needed.  */
#define SIEVE_LIMIT 300001

/* ------------- Helper data structures ------------- */
static int  *prime_list   = NULL; /* list of primes ≤ SIEVE_LIMIT      */
static int   prime_cnt    = 0;    /* number of primes in prime_list   */
static int  *prime_to_idx = NULL; /* map prime value → index in list */
static int  *prime_exp    = NULL; /* max exponent seen for each prime */

/* ------------- Caching of computed terms ------------- */
static seq_t *a_cache       = NULL; /* a(1) stored at a_cache[0]       */
static size_t cache_size    = 0;    /* allocated size of a_cache        */
static size_t cache_used    = 0;    /* highest n computed (1‑based)      */

/* ------------- Initialisation routine ------------- */
static void init_primes(void) {
    static char sieve[SIEVE_LIMIT + 1];
    int i, j;

    memset(sieve, 1, sizeof(sieve));
    sieve[0] = sieve[1] = 0;
    for (i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (sieve[i]) {
            for (j = i * i; j <= SIEVE_LIMIT; j += i)
                sieve[j] = 0;
        }
    }
    /* Count primes */
    for (i = 2; i <= SIEVE_LIMIT; ++i)
        if (sieve[i]) ++prime_cnt;
    /* Allocate prime list and mapping */
    prime_list   = (int *)malloc(prime_cnt * sizeof(int));
    prime_to_idx = (int *)malloc((SIEVE_LIMIT + 1) * sizeof(int));
    memset(prime_to_idx, -1, (SIEVE_LIMIT + 1) * sizeof(int));
    /* Fill lists */
    for (i = 2, j = 0; i <= SIEVE_LIMIT; ++i)
        if (sieve[i]) {
            prime_list[j]   = i;
            prime_to_idx[i] = j;
            ++j;
        }
    /* Allocate exponent array, initialise to 0 */
    prime_exp = (int *)calloc(prime_cnt, sizeof(int));
}

/* ------------- Cache management ------------- */
static void ensure_cache(size_t needed) {
    if (cache_size >= needed) return;
    size_t new_size = cache_size > 0 ? cache_size : 256;
    while (new_size < needed) new_size <<= 1;
    a_cache = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
    if (!a_cache) abort(); /* memory failure */
    cache_size = new_size;
}

/* ------------- Main function ------------- */
seq_t a(idx_t n) {
    static int initialised = 0;
    if (!initialised) {
        init_primes();
        initialised = 1;
    }
    if (n == 0) return 0;           /* undefined; return 0 for safety */
    /* Compute up to n if necessary */
    if (n > cache_used) {
        ensure_cache((size_t)n);
        for (size_t curr = cache_used; curr < (size_t)n; ++curr) {
            /* curr corresponds to n = curr+1 in 1‑based indexing */
            size_t nn = curr + 1;
            unsigned long long X = 3 * nn + 1;   /* ≤ SIEVE_LIMIT + 1 */
            unsigned long long temp = X;
            unsigned long long gcd_factor = 1;

            /* Factorisation of X */
            for (size_t pi = 0; pi < (size_t)prime_cnt && (unsigned long long)prime_list[pi] * (unsigned long long)prime_list[pi] <= temp; ++pi) {
                int p = prime_list[pi];
                int e = 0;
                while (temp % p == 0) {
                    ++e;
                    temp /= p;
                }
                if (e > 0) {
                    int idx = prime_to_idx[p];
                    int prev_e = prime_exp[idx];
                    int gcd_e = prev_e < e ? prev_e : e;
                    while (gcd_e--) gcd_factor *= (unsigned long long)p;
                    if (e > prev_e) prime_exp[idx] = e;
                }
            }
            if (temp > 1) { /* temp is prime */
                int p = (int)temp;
                int idx = prime_to_idx[p];
                int prev_e = prime_exp[idx];
                int gcd_e = prev_e < 1 ? prev_e : 1;
                if (gcd_e) gcd_factor *= (unsigned long long)p;
                if (prev_e < 1) prime_exp[idx] = 1;
            }

            unsigned long long ratio = X / gcd_factor;
            a_cache[curr] = (seq_t)ratio;
        }
        cache_used = n;
    }
    return a_cache[n - 1];
}
