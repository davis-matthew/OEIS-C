/*
 * A139937  Primes of the form 5x^2+69y^2.
 * https://oeis.org/A139937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 415, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 58 terms through n = 9286 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes of the form 5*x^2 + 69*y^2 by using the known
   residue classes modulo 1380 and testing each prime for
   representability with a brute‑force search of the quadratic
   form.  The function caches results to support arbitrary order
   of queries. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <stdlib.h>
#include "../utils/ipow_i32.h"
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

/* Residue classes of primes of that form (mod 1380). */
static const int RESIDUES[] = {
    5, 89, 149, 221, 281, 329, 341, 389, 401,
    521, 569, 641, 689, 701, 881, 941, 1049,
    1109, 1121, 1169, 1229, 1241, 1349
};
#define RES_COUNT (sizeof RESIDUES / sizeof RESIDUES[0])

/* Cached list of found primes. */
static seq_t *prime_cache = NULL;
static idx_t  cache_len  = 0;
static size_t cache_cap  = 0;

/* State for generating candidates in order of increasing value. */
static idx_t next_k = 0;      /* multiplier of the period 1380 */
static size_t next_r = 0;     /* index in the residue array */

/* Test whether p = 5*x^2 + 69*y^2 for some integers x, y. */
static bool has_representation(seq_t p)
{
    for (idx_t y = 0;; ++y) {
        seq_t y2   = (seq_t)y * (seq_t)y;
        seq_t part = (seq_t)69 * y2;
        if (part > p) break;

        seq_t rem = p - part;
        if (rem % (seq_t)5 != 0) continue;

        seq_t x2 = rem / (seq_t)5;
        seq_t x  = floor_sqrt(x2);
        if (x * x == x2) return true;
    }
    return false;
}

/* Generate primes until we have at least n terms. */
static void ensure_terms(idx_t n)
{
    while (cache_len < n) {
        /* Next candidate number in the residue classes. */
        seq_t candidate =
            ((seq_t)1380 * (seq_t)next_k) + (seq_t)RESIDUES[next_r];

        /* Advance to the following residue. */
        ++next_r;
        if (next_r == RES_COUNT) {
            next_r = 0;
            ++next_k;
        }

        if (!is_prime_big(candidate))
            continue;

        if (!has_representation(candidate))
            continue;

        /* Store the new prime. */
        grow_seq(&prime_cache, &cache_cap, cache_len + 1);
        prime_cache[cache_len++] = candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined, but safe */
    ensure_terms(n);
    return prime_cache[n - 1];
}
