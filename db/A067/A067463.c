/*
 * A067463  Primes p such that p+sigma(p+1) is prime.
 * https://oeis.org/A067463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   primes p such that p + sigma(p + 1) is prime
 * Range:     u8 n <= 12, u16 n <= 1098, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9985 (2026-11-03)
 * Helpers:   is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The logic below computes successive primes that satisfy the
   condition.  A static cache of found primes is used to allow
   calls in any order.  The routine is efficient for n up to
   at least 10 000. */

#include <stdlib.h>
#include <stdint.h>

#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

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

/* static cache of found primes (type seq_t) */
static seq_t *prime_cache = NULL;
static size_t prime_cache_len = 0;
static size_t prime_cache_cap = 0;

/* last prime examined for the next search */
static uint64_t last_prime_searched = 2;

/* Ensure the cache has room for at least 'needed' elements. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= prime_cache_cap)
        return;
    if (prime_cache_cap == 0)
        prime_cache_cap = 8;
    while (prime_cache_cap < needed)
        prime_cache_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, prime_cache, prime_cache_cap);
    prime_cache = tmp;
}

/* a(n): nth prime with the required property.  a(1) is the first
   such prime. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for n < 1, but safe */

    /* return precomputed value if available */
    if ((size_t)n <= prime_cache_len)
        return prime_cache[n - 1];

    /* generate more primes until we have at least n of them */
    while (prime_cache_len < (size_t)n)
    {
        /* find the next prime after the last one examined */
        uint64_t cand = next_prime(last_prime_searched + 1U);
        last_prime_searched = cand;

        /* compute sigma(cand + 1) */
        idx_t m = (idx_t)cand + 1ULL;
        seq_t sigma = sigma_of(m);

        /* test whether cand + sigma is prime */
        seq_t sum = (seq_t)cand + sigma;
        if (is_prime_big(sum))
        {
            /* store the candidate */
            ensure_cache_capacity(prime_cache_len + 1);
            prime_cache[prime_cache_len++] = (seq_t)cand;
        }
    }

    return prime_cache[n - 1];
}
