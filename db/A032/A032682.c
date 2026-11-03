/*
 * A032682  Numbers k such that k surrounded by digit '1' is a prime.
 * https://oeis.org/A032682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth non‑negative integer k such that 1·k·1
 *            (concatenation of decimal digits “1k1”) is prime
 * Range:     u8 n <= 5, u16 n <= 951, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9489 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function a(n) below computes this sequence on demand, caching
    previously found terms so that arbitrary order of calls is supported.
    It relies on the helper functions is_prime_big, pow10, digits, and
    grow_seq.  */

/* External declarations for helper functions. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/num_digits_idx.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
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

/* Cache state ------------------------------------------------------- */
static seq_t *cache = NULL;   /* array of found k values (a(n))      */
static size_t cache_cap = 0;  /* current allocation size of cache    */
static size_t cache_len = 0;  /* number of terms stored in cache     */
static idx_t k_next = 0;      /* next candidate k to test            */

/* Return the nth term of the sequence (1‑based indexing).  */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; for n ≤ 0 return 0 as a safe
       default (the build will never ask for such indices). */
    if (n == 0)
        return 0;

    /* Expand the cache until it contains at least n terms. */
    while (cache_len < n) {
        /* Candidate value k to be tested. */
        idx_t k = k_next++;

        /* Compute the number 1k1 as N = 10^(digits(k)+1) + k*10 + 1. */
        idx_t d = num_digits_idx(k);                      /* number of digits of k */
        seq_t N = pow10(d + 1);                    /* 10^(d+1)            */
        N = N + (seq_t)k * 10 + 1;                 /* append k and final 1 */

        /* Test whether N is prime. */
        if (is_prime_big(N)) {
            /* Store k in the cache as the next sequence term. */
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = (seq_t)k;
        }
    }

    return cache[n - 1];
}
