/*
 * A230351  Number of ordered ways to write n = p + q (q > 0) with p, 2*p^2 -
 *          1 and 2*q^2 - 1 all prime.
 * https://oeis.org/A230351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs (p,q) such that n = p + q, q > 0,
 *            and both p, 2*p*p - 1 and 2*q*q - 1 are prime
 * Range:     u8 n <= 13, u16 n <= 1042, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9941 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
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

/* Static storage for primes up to the maximum n seen so far. */
static uint64_t *prime_arr = NULL;
static size_t    prime_cnt = 0;
static idx_t     prime_limit = 0;

/* Ensure that prime_arr contains all primes ≤ limit. */
static void ensure_primes(idx_t limit) {
    if (prime_limit >= limit) return;
    uint64_t *new_arr = NULL;
    size_t    new_cnt = 0;
    build_primes_upto(limit, &new_arr, &new_cnt);
    if (prime_arr) free(prime_arr);
    prime_arr   = new_arr;
    prime_cnt   = new_cnt;
    prime_limit = limit;
}

/* OEIS A230351 */
seq_t a(idx_t n) {
    if (n <= 1) return 0;                /* no p < n when n <= 1 */
    idx_t up = n - 1;
    ensure_primes(up);

    seq_t count = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_arr[i];
        if (p >= n) break;
        uint64_t q = n - p;
        seq_t val_p = ((seq_t)2 * (seq_t)p * (seq_t)p) - (seq_t)1;
        if (!is_prime_big(val_p)) continue;
        seq_t val_q = ((seq_t)2 * (seq_t)q * (seq_t)q) - (seq_t)1;
        if (!is_prime_big(val_q)) continue;
        ++count;
    }
    return count;
}
