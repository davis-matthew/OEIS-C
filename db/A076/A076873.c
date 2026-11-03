/*
 * A076873  Smallest prime not less than sum of first n primes.
 * https://oeis.org/A076873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 156, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9227 (2026-11-03)
 * Helpers:   is_prime_u64, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest prime not less than sum of first n primes.
   a(n) defined as the smallest prime >= sum_{k=1}^{n} p_k,
   where p_k is the k-th prime. */
#include <stdlib.h>
#include <stdint.h>

#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/sieve_first_n_primes.h"
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

seq_t a(idx_t n)
{
    /* Cache the first n primes to avoid recomputation. */
    static idx_t cached_n = 0;
    static uint32_t *primes = NULL;
    static size_t primes_len = 0;

    if (n > (idx_t)cached_n) {
        /* Resize cache to accommodate the new n. */
        free(primes);                /* free previous cache, if any */
        sieve_first_n_primes(n, &primes, &primes_len);
        cached_n = n;
    }

    /* Compute the sum of the first n primes in seq_t space. */
    seq_t sum = 0;
    for (idx_t i = 0; i < n; ++i) {
        sum += (seq_t)primes[i];
    }

    /* Find the smallest prime not less than sum. */
    seq_t candidate = sum;
    while (!is_prime_big(candidate)) {
        ++candidate;
    }
    return candidate;
}
