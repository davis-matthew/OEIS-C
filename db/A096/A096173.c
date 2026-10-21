/*
 * A096173  Numbers k such that k^3+1 is an odd semiprime.
 * https://oeis.org/A096173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 986, u32 n <= 2086, u64 n <= 2086,
 *            u128 n <= 2086, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth even integer k such that k^3 + 1 is an odd semiprime
 *        (i.e. k + 1 and k^2 – k + 1 are both prime).  The function
 *        builds the sequence on demand, caching found terms.          */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* --- fast 64‑bit primality test (Miller–Rabin, deterministic for
 *      n < 3·10^12).  It works on values that fit in uint64_t.   */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2)                return false;
    static const uint64_t small_primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37
    };
    for (size_t i = 0; i < sizeof small_primes / sizeof *small_primes; ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    /* n is odd and > 37 */
    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0) {
        d >>= 1;
        ++s;
    }
    /* deterministic bases for n < 3,474,749,660,383 */
    const uint64_t bases[] = {2, 3, 5, 7, 11, 13};
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        __uint128_t x = 1, p = a, exp = d, mod = n;
        /* binary exponentiation */
        while (exp) {
            if (exp & 1) x = (x * p) % mod;
            p = (p * p) % mod;
            exp >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % mod;
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* Sequence generator ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for the offset */

    static seq_t *cache      = NULL;      /* found terms */
    static size_t cache_len  = 0;          /* number of stored terms   */
    static size_t cache_cap  = 0;          /* allocated capacity       */
    static uint64_t next_k   = 2;          /* next even candidate */

    /* Build up to the required index */
    while (cache_len < (size_t)n) {
        uint64_t k = next_k;
        next_k += 2;                      /* only even candidates    */

        /* k+1 must be prime */
        if (!is_prime_u64(k + 1)) continue;

        /* compute k^2 - k + 1  (fits into 64‑bit for all k up to
         * about 4·10^9, which is far beyond what we need for n ≤ 10000).
         */
        __uint128_t t = (__uint128_t)k * k - k + 1;
        uint64_t p2   = (uint64_t)t;       /* low 64 bits suffice */

        if (!is_prime_u64(p2)) continue;   /* k^2 - k + 1 not prime   */

        /* a valid term – store it in the cache */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {                     /* allocation failure */
                free(cache);
                cache = NULL;
                cache_len = cache_cap = 0;
                return (seq_t)0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = (seq_t)k;
    }

    return cache[(size_t)n - 1];
}
