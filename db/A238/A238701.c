/*
 * A238701  Number of primes p < n with q = floor((n-p)/4) and q^2 - 2 both
 *          prime.
 * https://oeis.org/A238701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 1291, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9383 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A238701:
   a(n) = #{p < n | p prime ∧ q = floor((n − p)/4) is prime ∧ q²−2 is prime}
   (offset 1) */

#include <stdint.h>
#include <stddef.h>
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

static uint64_t *prime_arr = NULL; /* array of primes < prime_lim */
static size_t   prime_cnt  = 0;
static idx_t    prime_lim   = 0;

/* Ensure we have all primes < n in prime_arr.  Grow the table if needed. */
static void
ensure_primes(idx_t n)
{
    if (prime_lim >= n)
        return;

    /* Allocate a slightly larger table to avoid frequent reallocations. */
    idx_t new_lim = n + 1000;
    uint64_t *p    = NULL;
    size_t     cnt = 0;

    build_primes_upto(new_lim, &p, &cnt);
    free(prime_arr);
    prime_arr = p;
    prime_cnt = cnt;
    prime_lim = new_lim;
}

seq_t a(idx_t n)
{
    /* For the defined offset, a(1) = 0; for n < 1 the function is unused. */
    if (n <= 1)
        return (seq_t)0;

    ensure_primes(n);

    seq_t count = 0;

    /* Iterate over all primes p < n. */
    for (size_t i = 0; i < prime_cnt && prime_arr[i] < n; ++i) {
        idx_t p = prime_arr[i];
        idx_t q = (n - p) / 4;          /* floor((n−p)/4) */
        if (q < 2)                       /* q must be at least 2 to be prime */
            continue;

        seq_t qseq = (seq_t)q;
        if (!is_prime_big(qseq))          /* q must be prime */
            continue;

        seq_t qsq   = qseq * qseq;
        seq_t test  = qsq - (seq_t)2;
        if (is_prime_big(test))
            ++count;
    }
    return count;
}
