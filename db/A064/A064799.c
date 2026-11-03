/*
 * A064799  Sum of n-th prime number and n-th composite number.
 * https://oeis.org/A064799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) + composite(n) (offset 1)
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 978 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
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

static seq_t *primes = NULL;
static size_t primes_cap = 0;
static size_t primes_len = 0;
static seq_t prime_candidate = 2;

static seq_t *composites = NULL;
static size_t comps_cap = 0;
static size_t comps_len = 0;
static seq_t composite_candidate = 4;

/* Ensure that at least n primes are stored. */
static void ensure_primes(idx_t n) {
    while (primes_len < (size_t)n) {
        while (!is_prime_big(prime_candidate))
            ++prime_candidate;
        if (primes_len == primes_cap) {
            size_t newcap = primes_cap ? primes_cap * 2 : 64;
            primes = mem_resize(seq_t, primes, newcap);
            primes_cap = newcap;
        }
        primes[primes_len++] = prime_candidate;
        ++prime_candidate;
    }
}

/* Ensure that at least n composite numbers are stored. */
static void ensure_composites(idx_t n) {
    while (comps_len < (size_t)n) {
        while (is_prime_big(composite_candidate))
            ++composite_candidate;
        if (comps_len == comps_cap) {
            size_t newcap = comps_cap ? comps_cap * 2 : 64;
            composites = mem_resize(seq_t, composites, newcap);
            comps_cap = newcap;
        }
        composites[comps_len++] = composite_candidate;
        ++composite_candidate;
    }
}

/* External entry point of the sequence. */
seq_t a(idx_t n) {
    if (n == 0) return 0;                         /* offset handling */
    ensure_primes(n);
    ensure_composites(n);
    return primes[n - 1] + composites[n - 1];
}
