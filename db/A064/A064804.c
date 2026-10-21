/*
 * A064804  Numbers 2*k such that phi(2*k + 2) < phi(2*k).
 * https://oeis.org/A064804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*k where φ(2*k + 2) < φ(2*k) (offset 1)
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* fast totient for unsigned long long values
 * works for numbers up to at least 10^12, far more than needed here   */
static unsigned long long phi_u64(unsigned long long n)
{
    unsigned long long result = n;
    if (n == 0) return 0;

    /* factor 2 */
    if ((n & 1ULL) == 0) {
        result -= result >> 1;          /* result *= 1/2   */
        while ((n & 1ULL) == 0) n >>= 1;
    }

    /* odd factors 3,5,7,... */
    for (unsigned long long p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            result -= result / p;
            while (n % p == 0) n /= p;
        }
    }

    /* a prime > sqrt(n) remains */
    if (n > 1) result -= result / n;
    return result;
}

/* main sequence routine ----------------------------------------------*/
seq_t a(idx_t n)
{
    /* static cache of already found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    /* next even number to inspect */
    static uint64_t next_k = 1;          /* k is such that m = 2*k   */

    /* generate terms until we have enough */
    while (cache_len < (size_t)n) {
        uint64_t k = next_k++;
        uint64_t m = 2ULL * k;          /* even number 2*k          */

        unsigned long long phi_m   = phi_u64(m);
        unsigned long long phi_mp2 = phi_u64(m + 2ULL);

        if (phi_mp2 < phi_m) {          /* condition φ(2k+2) < φ(2k) */
            /* grow cache if necessary */
            if (cache_len + 1 > cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)m;
        }
    }

    return cache[n - 1];
}
