/*
 * A099911  Primes of the form (p*(q-1) + (p-1)*q)/2, where p and q are
 *          consecutive odd primes.
 * https://oeis.org/A099911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)*prime(n+1) - (prime(n)+prime(n+1))/2) where
 *            prime(k) is the k‑th odd prime (prime(1)=3). The sequence
 *            consists of those values that are themselves prime. a(1) is the
 *            smallest such prime (11). The function uses memoisation and a
 *            pre‑computed list of primes
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 32 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

 /* Static state for memoisation */
static uint64_t *primes      = NULL;   /* array of odd primes */
static size_t   prime_cnt    = 0;      /* number of primes in the array */
static seq_t   *cache        = NULL;   /* cached sequence values */
static size_t   cache_len    = 0;      /* number of cached seq_t values */
static size_t   cache_cap    = 0;      /* capacity of the cache array */
static size_t   pair_idx     = 0;      /* current index in the prime array */
static bool     primes_ready = false;  /* flag whether primes have been generated */

/*
 * Return the n-th term of OEIS sequence A099911.
 * n is 1‑based (n >= 1).  Behaviour for n < 1 is undefined.
 */
seq_t a(idx_t n)
{
    if (n < 1)                      /* undefined for n < 1 */
        return 0;

    /* Initialise the prime list once.  The chosen limit is large
     * enough for at least the first 10 000 terms; it can be increased
     * if higher indices are required. */
    if (!primes_ready) {
        /* 3 000 000 is far beyond the 10 000‑th prime (~104729). */
        build_primes_upto((idx_t)3000000, &primes, &prime_cnt);
        primes_ready = true;
    }

    /* Extend cache until the requested term is available */
    while (cache_len < (size_t)n) {
        /* Ensure there is a consecutive pair left to examine */
        if (pair_idx + 1 >= prime_cnt)
            break;   /* silent failure if the prime list was too small */

        uint64_t p = primes[pair_idx];
        uint64_t q = primes[pair_idx + 1];

        /* Compute value: p*q - (p+q)/2 */
        seq_t val = (seq_t)p * q - ((seq_t)(p + q) / 2);

        /* Store the value if it is prime */
        if (is_prime_big(val)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap << 1;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp)   /* allocation failure */
                    return 0;
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }

        /* Move to next consecutive pair */
        pair_idx++;
    }

    return cache[n - 1];
}
