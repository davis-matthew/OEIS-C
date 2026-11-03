/*
 * A176585  Primes of the form n^3+Smallest square, (Smallest square >= n^3).
 * https://oeis.org/A176585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth prime of the form i^3 + ceil(sqrt(i^3))^2, where i runs over
   positive integers. The sequence starts with a(1)=2 for i=1, a(2)=17 for
   i=2, a(3)=269 for i=5, and so on. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
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

static seq_t *prime_cache      = NULL;   /* array of found primes */
static size_t prime_cache_len = 0;       /* number of cached primes        */
static size_t prime_cache_cap = 0;       /* allocated capacity             */
static idx_t  last_i           = 0;       /* last i processed (cumulative)  */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* defensive, not used          */

    /* Extend the cache until it contains at least n primes. */
    while ((idx_t)prime_cache_len < n) {

        /* Next integer i to test. */
        last_i++;
        seq_t i_seq = (seq_t)last_i;

        /* Compute i^3. */
        seq_t i_cubed = i_seq * i_seq * i_seq;

        /* Smallest integer >= sqrt(i^3). */
        seq_t t = floor_sqrt(i_cubed);
        if (t * t < i_cubed)                 /* ceil sqrt                */
            t++;

        /* Candidate prime = i^3 + t^2. */
        seq_t candidate = i_cubed + t * t;

        /* Test primality and cache if prime. */
        if (is_prime_big(candidate))
            append_candidate(&prime_cache, &prime_cache_len,
                             &prime_cache_cap, candidate);
    }

    /* Return the requested nth prime (1‑based index). */
    return prime_cache[n - 1];
}
