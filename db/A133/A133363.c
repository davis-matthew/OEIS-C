/*
 * A133363  Numbers m such that 1 + Sum_{k=1..m} 3*k is prime.
 * https://oeis.org/A133363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   m such that 1 + 3*m*(m+1)/2 is prime. a(n) returns the nth such
 *            m (a(1)=3). The function uses a static cache; it is fast for n
 *            up to about 20000
 * Range:     u8 n <= 4, u16 n <= 47, u32 n <= 5643, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*=== Includes ===========================================================*/
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/*=== Fast deterministic Miller–Rabin for 64‑bit integers ==================*/
static bool isPrime64(uint64_t n)
{
    /* Small cases and divisibility by small primes */
    if (n < 2)            return false;
    if (n % 2 == 0)       return n == 2;
    if (n % 3 == 0)       return n == 3;
    if (n % 5 == 0)       return n == 5;
    if (n % 7 == 0)       return n == 7;
    if (n % 11 == 0)      return n == 11;
    if (n % 13 == 0)      return n == 13;
    if (n % 17 == 0)      return n == 17;
    if (n % 19 == 0)      return n == 19;
    if (n % 23 == 0)      return n == 23;
    if (n % 29 == 0)      return n == 29;
    if (n % 31 == 0)      return n == 31;
    if (n % 37 == 0)      return n == 37;

    /* Write n-1 as d·2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* Deterministic bases for 64‑bit integers */
    const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __int128 x = 1;
        __int128 pow = a;
        uint64_t exp = d;
        while (exp) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }
        uint64_t xx = (uint64_t)x;
        if (xx == 1 || xx == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            __int128 xx128 = (__int128)xx * xx % n;
            xx = (uint64_t)xx128;
            if (xx == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/*=== Wrapper for generic seq_t =========================================*/
static bool is_prime_seq_t(seq_t val)
{
    /* seq_t may be up to 128‑bit.  Use the 64‑bit routine when it fits. */
    if (sizeof(seq_t) <= 8) {
        return isPrime64((uint64_t)val);
    } else {
        return is_prime_big(val);
    }
}

/*=== Main sequence function =============================================*/
seq_t a(idx_t n)
{
    /* Static cache of found m values, growing lazily. */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_m = 1;          /* last m tested + 1 */

    /* Produce terms until we have at least n of them. */
    while (cache_len < n) {
        idx_t m = next_m;
        /* Compute 1 + 3*m*(m+1)/2 using 128‑bit to avoid overflow. */
        __int128 temp = (__int128)m * (m + 1) / 2 * 3 + 1;
        seq_t val = (seq_t)temp;       /* cast to seq_t (fits, as val < 2^64) */

        if (is_prime_seq_t(val)) {
            /* Grow cache if needed. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {                /* allocation failure – return 0 */
                    if (cache) free(cache);
                    cache = NULL;
                    cache_len = cache_cap = 0;
                    return (seq_t)0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)m;
        }
        ++next_m;
    }

    /* a(1) is the first term, so a(n) is at index n-1. */
    return cache[n - 1];
}
