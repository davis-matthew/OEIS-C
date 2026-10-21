/*
 * A088036  Numbers k such that k^3 - 1 is divisible by a cube other than 1.
 * https://oeis.org/A088036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 1185, u32 n <= 1185, u64 n <= 1185,
 *            u128 n <= 1185, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A088036: a(n) = the n‑th positive integer k such that k^3-1 is
 * divisible by some cube greater than 1.  The function builds the sequence
 * by searching k in increasing order and storing the first “n” found.
 */

/* required types are supplied by the build system */
/* seq_t is an unsigned integer type; idx_t is an unsigned 64‑bit index */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* maximum prime we will generate – enough for k up to ~10^5             */
#define MAX_PRIME 200000

/* storage for small primes */
static unsigned char prime_sieve[MAX_PRIME + 1];
static unsigned int primes[MAX_PRIME];
static size_t prime_cnt = 0;

/* initialize primes once */
static void init_primes(void)
{
    if (prime_cnt != 0) return;
    for (size_t i = 2; i <= MAX_PRIME; ++i) prime_sieve[i] = 1;
    for (size_t i = 2; (size_t)i * i <= MAX_PRIME; ++i) {
        if (prime_sieve[i]) {
            for (size_t j = i * i; j <= MAX_PRIME; j += i)
                prime_sieve[j] = 0;
        }
    }
    for (size_t i = 2; i <= MAX_PRIME; ++i)
        if (prime_sieve[i]) primes[prime_cnt++] = (unsigned int)i;
}

/* test whether k satisfies the cube divisibility condition */
static bool qualifies(unsigned long long k)
{
    __int128 k128 = (__int128)k;
    __int128 n = k128 * k128 * k128 - (__int128)1;   /* k^3 - 1 */

    for (size_t i = 0; i < prime_cnt; ++i) {
        unsigned int p = primes[i];
        __int128 p3 = (__int128)p * p * p;           /* p^3  */
        if (p3 > n) break;                           /* no larger cube divisor is possible */

        if (n % p3 == 0) return true;                /* found a cube divisor */
    }
    return false;
}

/* dynamic cache of terms found so far */
static seq_t *cache = NULL;
static size_t cache_len = 0;          /* number of terms stored */
static size_t cache_cap = 0;          /* allocated capacity */

/* last k examined; start from 2 and increase as needed */
static unsigned long long current_k = 2;

/* ensure the cache has at least n terms stored */
static void grow_cache(size_t needed)
{
    if (cache_len >= needed) return;
    if (cache_cap == 0) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else {
        size_t new_cap = cache_cap * 2;
        while (new_cap < needed) new_cap *= 2;
        seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_buf) {
            cache = new_buf;
            cache_cap = new_cap;
        }
    }
    /* fill new slots lazily in the caller */
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n = 0, return 0 */
    init_primes();
    grow_cache(n);

    /* compute terms until we have at least n */
    while (cache_len < n) {
        ++current_k;
        if (qualifies(current_k)) {
            cache[cache_len++] = (seq_t)current_k;
        }
    }
    return cache[n - 1];
}
