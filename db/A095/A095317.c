/*
 * A095317  Primes in whose binary expansion the number of 1 bits is <= number
 *          of 0 bits minus 2.
 * https://oeis.org/A095317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 933 (2026-11-03)
 * Helpers:   bit_length, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes with (#1 bits) ≤ (#0 bits) – 2 in binary */
#include <stdlib.h>
#include <stdbool.h>
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

static unsigned popcount_seq(seq_t x)
{
    /* Fast popcount for ≤ 64‑bit values */
    if (sizeof(seq_t) <= sizeof(uint64_t))
        return __builtin_popcountll((uint64_t)x);

    /* Generic popcount using the “bitwise clean‑up” trick */
    seq_t t = x;
    unsigned cnt = 0;
    while (t) {
        t &= t - 1;
        ++cnt;
    }
    return cnt;
}

static void grow_cache(size_t need, size_t *cap, seq_t **arr)
{
    if (*cap < need) {
        size_t newcap = (*cap > 0) ? (*cap << 1) : 4;
        while (newcap < need) newcap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, newcap);
        *arr = tmp;
        *cap = newcap;
    }
}

seq_t a(idx_t n)
{
    /* Static memoized cache of found terms */
    static seq_t *cache   = NULL;
    static size_t  clen   = 0;
    static size_t  ccap   = 0;
    static seq_t next     = 3;          /* first odd prime candidate */

    if (n == 0) return 0;  /* undefined for n = 0, but keep safe */

    /* grow cache until we have at least n elements */
    while (clen < (size_t)n) {
        seq_t cand = next;

        /* primality test suitable for the size of seq_t */
        if (!is_prime_big(cand)) {
            next += 2;
            continue;
        }

        unsigned ones = popcount_seq(cand);
        unsigned bits = bit_length(cand);

        /* Condition: ones ≤ zeros – 2  <=>  bits ≥ 2*ones + 2 */
        if (bits >= 2u * ones + 2u) {
            grow_cache(clen + 1, &ccap, &cache);
            cache[clen++] = cand;
        }
        next += 2;  /* skip even numbers only after 2 */
    }
    return cache[n - 1];
}
