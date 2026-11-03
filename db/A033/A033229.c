/*
 * A033229  Primes of form x^2+42*y^2.
 * https://oeis.org/A033229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 786, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 53 terms through n = 8537 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the *n*‑th prime that can be written as
   x^2 + 42 * y^2.  
   On the web page the sequence is:
      a(1) = 43, a(2) = 67, a(3) = 163, …  (OEIS A033229)
   A prime p has this form iff p ≡ 1, 25, 43, 67, 121 or 163 (mod 168).
   The routine keeps a cache so that calls may be made in any order. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

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
    if (n == 0) return 0;  /* undefined for offset 1, but safe return */

    /* static cache of found terms */
    static seq_t *cache = NULL;      /* dynamically sized array of primes   */
    static size_t cache_cap = 0;     /* current allocation (number of items) */
    static idx_t  found = 0;         /* number of primes stored              */
    static seq_t last_candidate = 1; /* value of the last examined number    */

    while (found < n)
    {
        /* Advance to the next integer */
        last_candidate = last_candidate + 1;

        /* Skip numbers not in the allowed residue classes mod 168 */
        seq_t r = last_candidate % 168;
        if (!(r == 1 || r == 25 || r == 43 || r == 67 || r == 121 || r == 163))
            continue;

        /* Test primality; for arbitrary‑precision values we use the helper */
        if (!is_prime_big(last_candidate))
            continue;

        /* Ensure space in the cache */
        if ((size_t)(found + 1) > cache_cap)
            grow_seq(&cache, &cache_cap, (size_t)(found + 3)); /* allocate a bit extra */

        cache[found] = last_candidate;
        ++found;
    }

    return cache[n - 1];
}
