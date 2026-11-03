/*
 * A088297  Primes in which no digit is coprime to its neighbors.
 * https://oeis.org/A088297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-03)
 * Helpers:   gcd_u32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates terms of OEIS A088297: primes in which no digit is
   coprime to its neighbors and no digit is 0 or 1.  The function
   returns the n-th such prime, with a(1)=2. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd_u32.h"
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

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* Check whether a prime has no digit 0/1 and each adjacent pair shares a
   common factor greater than 1. */
static bool is_special_prime(seq_t p)
{
    /* Single‑digit primes */
    if (p < (seq_t)10)
        return p == 2 || p == 3 || p == 5 || p == 7;

    seq_t tmp    = p;
    unsigned prev = 0;   /* previous (more significant) digit */
    bool first   = true;

    while (tmp > (seq_t)0) {
        unsigned cur = (unsigned)(tmp % (seq_t)10);

        /* Reject digits 0 or 1 */
        if (cur <= 1)
            return false;

        if (!first && gcd_u32(cur, prev) == 1)
            return false;        /* adjacent digits coprime */

        prev   = cur;
        first  = false;
        tmp   /= (seq_t)10;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* offset 0 not used */

    static seq_t *cache          = NULL;
    static size_t cache_len      = 0;
    static size_t cache_cap      = 0;
    static seq_t next_candidate  = 2;   /* next integer to test */

    /* Grow cache until it contains n primes of the desired type. */
    while (cache_len < (size_t)n) {
        seq_t cand = next_candidate;

        /* Advance to the next odd (or 3 after 2) candidate. */
        if (cand <= (seq_t)2)
            next_candidate = (seq_t)3;
        else
            next_candidate = cand + (seq_t)2;

        if (!is_prime_big(cand))
            continue;          /* must be prime */

        if (is_special_prime(cand))
            append_candidate(&cache, &cache_len, &cache_cap, cand);
    }

    return cache[(size_t)n - 1];
}
