/*
 * A141773  Primes of the form x^2 + 9*x*y - y^2 (as well as of the form 9*x^2
 *          + 11*x*y + y^2).
 * https://oeis.org/A141773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A141773:
   primes p that can be written as
        p = x^2 + 9*x*y – y^2
   or
        p = 9*x^2 + 11*x*y + y^2
   This function generates primes in order and tests each for
   representability using the discriminant equations derived
   from the two quadratic forms.  The algorithm is a simple
   incremental sieve; it does not rely on any hard‑coded table.  */

/* Commented includes required for indirect utility functions.
   The build system will provide the corresponding headers. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

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

/* Test whether a prime p can be represented by one of the two forms. */
static bool
is_represented(seq_t p)
{
    /* The discriminant equations for each form: */
    /* form 1:  Δ = 85*y^2 + 4*p        ;  x = (-9y ± √Δ) / 2   */
    /* form 2:  Δ = 85*y^2 + 36*p       ;  x = (-11y ± √Δ) /18  */

    seq_t sqrt_p = floor_sqrt(p);
    idx_t limit = (idx_t)sqrt_p;   /* y ≤ √p is sufficient for the search */

    for (idx_t y = 0; y <= limit; ++y) {
        seq_t y_s   = (seq_t)y;
        seq_t y_sq  = y_s * y_s;

        /* ----- form 1 ----- */
        seq_t v1 = 85 * y_sq + 4 * p;
        seq_t r1 = floor_sqrt(v1);
        if (r1 * r1 == v1) {
            /* (√Δ + 9y) must be even for integer x */
            if ( ((r1 + 9 * y_s) & 1) == 0 )
                return true;
        }

        /* ----- form 2 ----- */
        seq_t v2 = 85 * y_sq + 36 * p;
        seq_t r2 = floor_sqrt(v2);
        if (r2 * r2 == v2) {
            /* (-11y ± √Δ) must be divisible by 18.
               Equivalent to (√Δ – 11y) or (√Δ + 11y) ≡ 0 (mod 18). */
            if ( ((r2 - 11 * y_s) % 18) == 0 ||
                 ((r2 + 11 * y_s) % 18) == 0 )
                return true;
        }
    }
    return false;
}

/* Main sequence function: returns the n-th prime with the required
   representation.  Results are cached so that arbitrary order of
   calls is handled efficiently. */
seq_t
a(idx_t n)
{
    /* Static cache of discovered primes */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static idx_t cand   = 2;        /* next integer to test for primality */

    while (len < n) {
        for (;;) {
            /* Skip non‑primes quickly */
            if (!is_prime_big((seq_t)cand)) { ++cand; continue; }

            /* Prime found: check representability */
            if (is_represented((seq_t)cand)) {
                if (len == cap)
                    grow_seq(&cache, &cap, 1);   /* enlarge cache if needed */
                cache[len++] = (seq_t)cand;
                ++cand;                       /* move past the prime just used */
                break;                         /* go back to outer while */
            }
            ++cand;
        }
    }
    return cache[n - 1];
}
