/*
 * A054906  Smallest number x such that sigma(x+2n) = sigma(x)+2n (first
 *          definition).
 * https://oeis.org/A054906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest prime p such that p+2n is also prime
 * Range:     u8 n <= 262, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 115 terms through n = 9272 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_seq, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a precomputed list of primes up to a fixed limit.
   For each call it scans this list until it finds a p with p+2n prime.
   If the limit is insufficient, it continues by testing successive
   odd numbers beyond the sieve limit.  */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_seq.h"
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

static const idx_t PRIME_SIEVE_LIMIT = 2000000; /* sufficient for moderate n */
static bool primes_built = false;
static uint64_t *primes = NULL;
static size_t prime_count = 0;

/* Helper to ensure the prime table is built once. */
static void ensure_primes(void)
{
    if (!primes_built) {
        build_primes_upto(PRIME_SIEVE_LIMIT, &primes, &prime_count);
        primes_built = true;
    }
}

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;  /* undefined for offset 1, but safe return */

    ensure_primes();

    /* Compute 2n as a seq_t value.  */
    seq_t t = ((seq_t)n) * 2;

    /* Scan primes from the pre‑computed list.  */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        seq_t sum = t + (seq_t)p;
        if (is_prime_big(sum)) {
            return (seq_t)p;
        }
    }

    /* In the unlikely event that the table is exhausted,
       continue by testing odd candidates beyond the table.  */
    uint64_t candidate = primes[prime_count - 1] + 2;
    for (;;) {
        if (is_prime_seq((seq_t)candidate)) {
            seq_t sum = t + (seq_t)candidate;
            if (is_prime_big(sum)) {
                return (seq_t)candidate;
            }
        }
        candidate += 2;
    }
}
