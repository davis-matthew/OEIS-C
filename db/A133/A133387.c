/*
 * A133387  Greatest prime p such that 6*n-p is prime.
 * https://oeis.org/A133387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the greatest prime p ≤ 6*n such that (6*n – p) is prime.
 *            The function uses a static prime table that is rebuilt for the
 *            largest limit seen so far, allowing calls with arbitrary n order
 * Range:     u8 n <= 43, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9625 (2026-11-03)
 * Helpers:   build_primes, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#include "../utils/build_primes.h"
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

static idx_t *prime_list   = NULL;   /* array of primes up to prime_limit */
static size_t prime_count  = 0;      /* number of primes in prime_list   */
static idx_t prime_limit   = 0;      /* current maximum limit covered    */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset starts at 1 */

    /* limit for the sieve: largest prime that could appear */
    idx_t limit = (idx_t)6 * n;

    /* Rebuild the prime table if necessary */
    if (limit > prime_limit) {
        free(prime_list);
        build_primes(limit, &prime_list, &prime_count);
        prime_limit = limit;
    }

    /* Binary search to find the largest prime ≤ limit */
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (prime_list[mid] <= limit)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo == 0) return 0;       /* should never happen (2 is prime) */
    size_t top = lo - 1;         /* index of largest prime ≤ limit */

    /* Search downward for the required pair (p, 6*n‑p) */
    size_t i = top;
    while (1) {
        idx_t p = prime_list[i];
        seq_t diff = (seq_t)6 * (seq_t)n - (seq_t)p;
        if (diff >= (seq_t)2 && is_prime_big(diff))
            return (seq_t)p;
        if (i == 0) break;
        --i;
    }

    return 0;   /* should never be reached */
}
