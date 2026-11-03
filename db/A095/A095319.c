/*
 * A095319  Primes in whose binary expansion the number of 1 bits is <= 3 +
 *          number of 0 bits.
 * https://oeis.org/A095319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth prime p such that 2·popcount(p) ≤ bitlen(p)+3
 *            (popcount = number of 1‑bits in binary expansion, bitlen =
 *            number of bits)
 * Range:     u8 n <= 45, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   bit_length, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/bit_length.h"
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

static unsigned popcount_seq_t(seq_t x)
{
    unsigned count = 0;
    while (x != (seq_t)0) {
        count += (unsigned)(x & (seq_t)1);
        x >>= 1;
    }
    return count;
}

static bool qualifies(seq_t p)
{
    unsigned bc = bit_length(p);
    unsigned pc = popcount_seq_t(p);
    return (2 * pc <= bc + 3);
}

/* static cache for the sequence */
static seq_t *cache     = NULL;   /* array of qualifying primes */
static size_t cache_len = 0;      /* number of cached terms */
static size_t cache_cap = 0;      /* capacity of the cache array   */
static seq_t  last_checked = (seq_t)0;  /* last candidate that was examined */

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of range, not used */

    /* Grow the cache until it contains at least n terms */
    while (cache_len < n) {
        /* Find the next candidate integer */
        seq_t cand;
        if (last_checked < (seq_t)2) {
            cand = (seq_t)2;
        } else {
            cand = last_checked + ((last_checked == (seq_t)2) ? (seq_t)1 : (seq_t)2);
            /* Ensure an odd candidate after 2 */
            if (cand > (seq_t)2 && (cand & (seq_t)1) == (seq_t)0)
                cand += (seq_t)1;
        }

        /* Examine the candidate */
        if (is_prime_big(cand) && qualifies(cand)) {
            /* Store it */
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 4;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = cand;
        }

        /* Prepare for the next iteration */
        last_checked = cand;
    }

    return cache[n - 1];
}
