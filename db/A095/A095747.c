/*
 * A095747  Maximally asymmetric odd primes in binary.
 * https://oeis.org/A095747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th maximally asymmetric odd prime in binary (primes
 *            whose binary representation has 1 at both ends and for every
 *            interior mirrored pair the bits differ)
 * Range:     u8 n <= 20, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-03)
 * Helpers:   bit_length, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/bit_length.h"
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

/* static cache of already found terms */
#define CACHE_MAX 20000
static seq_t cache[CACHE_MAX];
static idx_t cache_len = 0;
static idx_t next_candidate = 3;   /* next odd number to test */

/* Test whether p satisfies the maximal asymmetry condition. */
static bool is_asymmetric(seq_t p)
{
    unsigned len = bit_length(p);
    /* All odd primes have 1 at both ends; the outer pair is allowed to be equal.
       For every other mirrored pair the bits must differ. */
    for (unsigned i = 1; i < len - 1 - i; ++i) {
        seq_t left  = (p >> i) & 1;
        seq_t right = (p >> (len - 1 - i)) & 1;
        if (left == right)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Build up the cache until it contains at least n terms */
    while (cache_len < n) {
        seq_t cand = (seq_t)next_candidate;
        if (is_prime_big(cand) && is_asymmetric(cand)) {
            if (cache_len < CACHE_MAX)
                cache[cache_len++] = cand;
        }
        /* advance to next odd candidate */
        next_candidate += 2;
    }
    return cache[n - 1];
}
