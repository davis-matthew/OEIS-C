/*
 * A227909  Number of ways to write 2*n = p + q with p, q and (p-1)*(q+1) - 1
 *          all prime.
 * https://oeis.org/A227909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 313, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9928 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Searches all prime pairs (p,q) with p+q=2*n and checks that
 * (p-1)*(q+1)-1 is prime, counting each ordered pair separately.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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

/* Binary search: true if 'val' is in sorted primes[0..cnt-1] */
static bool _is_prime_in_list(uint64_t val, uint64_t *primes, size_t cnt)
{
    size_t lo = 0, hi = cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        uint64_t midval = primes[mid];
        if (midval == val) return true;
        if (midval < val) lo = mid + 1;
        else hi = mid;
    }
    return false;
}

/* Simple sieve up to 'limit' (inclusive).  Allocates primes in *out and
 * sets *cnt to the number of primes found. */
static void _simple_sieve(idx_t limit, uint64_t **out, size_t *cnt)
{
    uint64_t *is_comp = (uint64_t *)calloc((size_t)limit + 1, sizeof(uint64_t));
    if (!is_comp) { *cnt = 0; *out = NULL; return; }

    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (uint64_t j = i * i; j <= limit; j += i) {
                is_comp[j] = 1;
            }
        }
    }

    *cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) ++(*cnt);
    }

    uint64_t *arr = (uint64_t *)malloc(*cnt * sizeof(uint64_t));
    if (!arr) {
        free(is_comp);
        *cnt = 0;
        *out = NULL;
        return;
    }

    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) arr[idx++] = i;
    }

    free(is_comp);
    *out = arr;
}

/* Cache of primes up to the largest requested limit. */
static uint64_t *prime_cache = NULL;
static size_t prime_cnt   = 0;
static idx_t   prime_max  = 0;

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    idx_t limit = 2 * n;                 /* target sum 2*n */

    /* Rebuild the prime cache if needed. */
    if (limit > prime_max) {
        free(prime_cache);
        _simple_sieve(limit, &prime_cache, &prime_cnt);
        prime_max = limit;
    }

    seq_t count = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_cache[i];
        if (p > limit) break;

        uint64_t q = limit - p;
        if (!_is_prime_in_list(q, prime_cache, prime_cnt))
            continue;

        seq_t val = ((seq_t)(p - 1)) * ((seq_t)(q + 1)) - 1;
        if (is_prime_big(val)) ++count;
    }
    return count;
}
