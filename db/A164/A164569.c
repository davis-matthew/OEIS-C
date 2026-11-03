/*
 * A164569  Primes p such that 11*p+8 are prime numbers.
 * https://oeis.org/A164569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that 11*p+8 is prime
 * Range:     u8 n <= 2, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    static seq_t next_candidate = 3;          /* first odd prime to test   */
    static idx_t  count        = 0;          /* primes found so far      */

#ifdef __cplusplus
    static std::vector<seq_t> cacheVec;      /* cache of qualifying primes */
#else
    static seq_t *cache   = NULL;            /* cache array              */
    static size_t capacity = 0;              /* allocated size           */
#endif

    while (count < n) {
        seq_t p = next_candidate;
        next_candidate += 2;                 /* skip even numbers */

        if (!is_prime_big(p))
            continue;                       /* compound, skip     */

        seq_t q = (seq_t)11 * p + (seq_t)8;
        if (!is_prime_big(q))
            continue;                       /* condition not met  */

#ifdef __cplusplus
        cacheVec.push_back(p);
#else
        if (count + 1 > capacity) {
            size_t new_cap = capacity ? 2 * capacity : 16;
            while (new_cap <= count)
                new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            capacity = new_cap;
        }
        cache[count] = p;
#endif
        count++;
    }

#ifdef __cplusplus
    return cacheVec[n - 1];
#else
    return cache[n - 1];
#endif
}
