/*
 * A039638  Fixed point of "k -> k/2 or (k-1)/2 until result is prime",
 *          starting with prime(n)-1.
 * https://oeis.org/A039638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 7600, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 92 terms through n = 9411 (2026-11-03)
 * Helpers:   build_primes, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fixed point of k → k/2 if k is even, (k-1)/2 if k is odd, starting
   from prime(n)-1 until a prime (or 1) is reached.  This implements
   OEIS sequence A039638.  */
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

#include <stdint.h>
#include <math.h>
#include "../utils/build_primes.h"
#include "../utils/is_prime_u64.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
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

/* Cached list of the first primes.  This is updated lazily when more
   primes are needed.  */
static idx_t *prime_cache = NULL;
static size_t prime_cnt   = 0;
static idx_t prime_limit  = 0;

/* Ensure that at least n primes are available in prime_cache.  */
static void
ensure_primes(idx_t n)
{
    if (prime_cnt >= n)
        return;

    /* Compute a reasonable upper bound for the nth prime and build the
       cache up to that bound (with a small safety margin).  */
    idx_t lim = _prime_limit_for_idx(n) + 100;

    if (prime_cache != NULL && prime_limit >= lim)
        return;

    free(prime_cache);
    build_primes(lim, &prime_cache, &prime_cnt);
    prime_limit = lim;
}

/* *********************************************************************
   Main sequence function.
   ********************************************************************* */
seq_t
a(idx_t n)
{
    /* n ≥ 1 (offset 1) */
    ensure_primes(n);

    /* Starting value: prime(n) – 1 */
    seq_t k = (seq_t)prime_cache[n - 1] - (seq_t)1;

    /* Handle the trivial case directly */
    if (k <= (seq_t)1)
        return k;

    /* Iterate until a prime (or 1) is obtained.  */
    while (!is_prime_big(k)) {
        if (k <= (seq_t)1)
            return (seq_t)1;
        if (k & (seq_t)1)           /* odd */
            k = (k - (seq_t)1) >> 1;
        else                        /* even */
            k >>= 1;
    }
    return k;
}
