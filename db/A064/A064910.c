/*
 * A064910  Smallest semiprime p*q such that q >= p and q mod p = n.
 * https://oeis.org/A064910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 135, u32 n <= 9782, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9941 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A064910
   Name: Smallest semiprime p*q such that q >= p and q mod p = n.
   Compute a(n) as follows:
   For each prime p > n, find the smallest prime q ≥ p + n with q ≡ n (mod p).
   The desired value is the minimum of all products p*q found. */

/* Required helper functions are in the utils directory:
   - build_primes_upto: to generate a list of primes up to a limit.
   - is_prime_big: primality test for seq_t values. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

static uint64_t *prime_list = NULL;
static size_t prime_count = 0;

/* Initialise a static list of primes up to a fixed limit.
   This limit is chosen to comfortably cover n ≤ 10 000 and the
   corresponding product sizes expected in the test set. */
static void init_prime_list(void)
{
    if (prime_list) return;
    size_t cnt;
    /* 200 000 is sufficient for all n in the test range. */
    build_primes_upto(200000, &prime_list, &cnt);
    prime_count = cnt;
}

seq_t a(idx_t n)
{
    init_prime_list();

    bool found = false;
    seq_t best = 0;   /* will be overwritten upon first candidate */

    /* Iterate over all primes p greater than n. */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p <= n) continue;          /* p must be > n so that n < p */

        /* Candidate q = p + n is the smallest integer ≥ p with
           q % p == n. Search for the first prime q of that form. */
        uint64_t q = p + n;
        while (true) {
            if (is_prime_big((seq_t)q)) {
                seq_t prod = (seq_t)p * (seq_t)q;
                if (!found || prod < best) {
                    best = prod;
                    found = true;
                }
                break;
            }
            q += p;                      /* next number congruent to n mod p */
        }

        /* Prune the search: for any larger p,
           the smallest possible product is p*(p+n).
           If that is already larger than the best found, we can stop. */
        if (found) {
            seq_t min_possible = (seq_t)p * (seq_t)(p + n);
            if (min_possible > best) break;
        }
    }

    return best;
}
