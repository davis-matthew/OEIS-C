/*
 * A046927  Number of ways to express 2n+1 as p+2q where p and q are primes.
 * https://oeis.org/A046927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3411, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A046927
   Number of ways to express 2n+1 as p + 2q with p and q prime.
   The function is memoised and works for any build where
   seq_t is an unsigned integer type or an arbitrary‑precision
   integer class.  For C++ builds new/​delete are used for dynamic
   arrays to ensure proper construction of objects.  For C builds
   malloc/realloc are used. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- Prime sieve data ----------
   prime_flag[i] == 1  iff i is prime
   prime_list   = array of all primes up to current limit
   prime_count  = number of primes in prime_list
   prime_limit  = current largest number for which prime_flag is valid
   ------------------------------------------------- */
static uint8_t  *prime_flag  = NULL;
static idx_t    prime_limit = 0;
static uint32_t *prime_list  = NULL;
static size_t   prime_count  = 0;

/* Rebuilds the primality flag array and prime list up to new_limit.
   The whole sieve is regenerated each time this function is called,
   which is efficient enough for n <= 10 000. */
static void extend_sieve(idx_t new_limit)
{
    if (new_limit <= prime_limit) return;

    size_t sz = (size_t)(new_limit + 1);
    uint8_t *f = (uint8_t *)realloc(prime_flag, sz);
    if (!f) exit(1);
    prime_flag = f;

    for (idx_t i = 0; i <= new_limit; ++i) prime_flag[i] = 1;
    if (new_limit >= 0) prime_flag[0] = 0;
    if (new_limit >= 1) prime_flag[1] = 0;

    for (idx_t i = 2; i * i <= new_limit; ++i) {
        if (prime_flag[i]) {
            for (idx_t j = i * i; j <= new_limit; j += i)
                prime_flag[j] = 0;
        }
    }

    size_t cnt = 0;
    for (idx_t i = 2; i <= new_limit; ++i)
        if (prime_flag[i]) ++cnt;

    uint32_t *l = (uint32_t *)realloc(prime_list, cnt * sizeof(uint32_t));
    if (!l) exit(1);
    prime_list = l;

    cnt = 0;
    for (idx_t i = 2; i <= new_limit; ++i)
        if (prime_flag[i]) prime_list[cnt++] = (uint32_t)i;

    prime_count = cnt;
    prime_limit = new_limit;
}

/* ---------- Memoised sequence storage ----------
   cache[i] holds a(i) once it has been computed.
   computed_up_to is the first index that has NOT yet been computed.  */
static seq_t *cache          = NULL;
static idx_t  cache_size     = 0;
static idx_t  computed_up_to = 0;

/* Ensure cache can hold indices up to n.  Works for both C and C++. */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;
    idx_t new_size = n + 1;

#ifdef __cplusplus
    seq_t *new_cache = new seq_t[new_size];
    if (cache) {
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
    }
    for (idx_t i = cache_size; i < new_size; ++i)
        new_cache[i] = 0;
    cache = new_cache;
#else
    seq_t *c = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!c) exit(1);
    for (idx_t i = cache_size; i < new_size; ++i)
        c[i] = 0;
    cache = c;
#endif

    cache_size = new_size;
}

/* ---------- Sequence function ---------- */
seq_t a(idx_t n)
{
    /* Expand data structures to cover requested index. */
    ensure_cache(n);
    if (n < computed_up_to) return cache[n];

    /* Ensure we have all primes up to 2*n+1 for the sieving needed. */
    extend_sieve((idx_t)(2 * n + 1));

    for (idx_t k = computed_up_to; k <= n; ++k) {
        seq_t count = 0;
        for (size_t pi = 0; pi < prime_count; ++pi) {
            idx_t q = prime_list[pi];
            if (q > k) break;                 /* q cannot exceed k   */
            idx_t p = (idx_t)2 * k + 1 - 2 * q;
            if (p >= 2 && prime_flag[p]) ++count;
        }
        cache[k] = count;
    }
    computed_up_to = n + 1;
    return cache[n];
}
