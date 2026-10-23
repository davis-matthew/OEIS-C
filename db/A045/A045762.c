/*
 * A045762  Numbers k such that 2^k - 1 is not a prime.
 * https://oeis.org/A045762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = the n-th non‑Mersenne exponent; i.e. the n-th integer k
 *            (starting k = 0) for which 2^k − 1 is not prime
 * Range:     u8 n <= 244, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* sequence values: a(1)=0, a(2)=1, a(3)=4, a(4)=6, ...   */

/* Note: the list 'mersenne_prime_exponents' contains all known exponents e with 2^e − 1 a prime
   up to 20 000, which is sufficient for n up to about 10 000.  The list is declared
   as a static constant and searched linearly. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Constants: known exponents that generate Mersenne primes               */
/* --------------------------------------------------------------------- */
static const uint64_t mersenne_prime_exponents[] = {
    2ULL,3ULL,5ULL,7ULL,13ULL,17ULL,19ULL,31ULL,61ULL,89ULL,
    107ULL,127ULL,521ULL,607ULL,1279ULL,2203ULL,2281ULL,
    3217ULL,4253ULL,4423ULL,9689ULL,9941ULL,11213ULL,11579ULL,
    12907ULL,13063ULL,16057ULL,16513ULL,17161ULL,17203ULL,
    18433ULL,19661ULL
};
static const size_t mersenne_prime_exponents_len =
    sizeof(mersenne_prime_exponents) / sizeof(mersenne_prime_exponents[0]);

/* Linear search for membership test */
static bool is_mersenne_prime_exponent(uint64_t k)
{
    for (size_t i = 0; i < mersenne_prime_exponents_len; ++i) {
        uint64_t e = mersenne_prime_exponents[i];
        if (e == k) return true;
        if (e > k) break;
    }
    return false;
}

/* --------------------------------------------------------------------- */
/* Cache for computed values (dynamic array of seq_t)                    */
/* --------------------------------------------------------------------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_k = 0;          /* next k to consider */

/* Ensure the cache has at least 'min_cap' elements of capacity */
static void ensure_cache_cap(size_t min_cap)
{
    if (cache_cap < min_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < min_cap) new_cap = min_cap;
        seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!newp) abort();
        cache = newp;
        cache_cap = new_cap;
    }
}

/* --------------------------------------------------------------------- */
/* Public sequence function                                            */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1, safe guard */

    size_t target = (size_t)n;     /* target length of cache */

    while (cache_len < target) {
        uint64_t k = next_k++;     /* candidate exponent */

        /* 0 and 1 are always included; for k>=2 skip if 2^k−1 is prime */
        if (!is_mersenne_prime_exponent(k)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)k;
        }
    }
    return cache[n - 1];
}
