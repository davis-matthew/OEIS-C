/*
 * A038987  Primes p such that 97 is a square mod p.
 * https://oeis.org/A038987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 941 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A038987: Primes p such that 97 is a quadratic residue modulo p
   a(1) = 2, a(2) = 3, a(3) = 11, a(4) = 31, …                                     */
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

seq_t a(idx_t n)
{
    /* Pre‑compute quadratic residues modulo 97 */
    static bool residues[97];
    static bool init = false;

    /* Cache of primes up to a generous upper bound */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static uint64_t last_known = 2;   /* last prime we have processed */

    if (!init)
    {
        /* Marks all squares mod 97 */
        for (int i = 0; i < 97; ++i)
            residues[(i * i) % 97] = true;

        /* Enough to cover the first ~10k terms (≈200k entries) */
        uint64_t limit = 2000000;
        build_primes_upto(limit, &primes, &primes_cnt);
        last_known = primes[primes_cnt - 1];
        init = true;
    }

    /* Count satisfied primes while iterating over the pre‑computed list */
    size_t count = 0;
    for (size_t i = 0; i < primes_cnt; ++i)
    {
        uint64_t p = primes[i];
        bool good = false;
        if (p == 2 || p == 97)
            good = true;
        else if (residues[p % 97])
            good = true;

        if (good)
        {
            ++count;
            if (count == n)
                return (seq_t)p;
        }
    }

    /* If we run out of pre‑computed primes, keep searching further */
    uint64_t p = last_known + 1;
    while (true)
    {
        /* Find the next prime greater than p */
        while (!is_prime_big((seq_t)p))
            ++p;
        last_known = p;

        bool good = false;
        if (p == 97)
            good = true;
        else if (residues[p % 97])
            good = true;

        if (good)
        {
            ++count;
            if (count == n)
                return (seq_t)p;
        }
        ++p;
    }
}
