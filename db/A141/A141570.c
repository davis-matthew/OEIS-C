/*
 * A141570  Primes of the form 2*3*5*7*n+83.
 * https://oeis.org/A141570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime of the form 210·k + 83, with k ≥ 0
 * Range:     u8 n <= 1, u16 n <= 135, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 55 terms through n = 928 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
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

/* External helper functions */
extern bool is_prime_big(seq_t n);
extern void grow_seq(seq_t **arr, size_t *cap, size_t need);

/* Return the n-th term of OEIS A141570.
 * n must be ≥ 1. For n < 1 the function returns 0. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Cached primes computed so far and state of enumeration. */
    static seq_t *cache   = NULL;   /* array of primes 210*k+83 found */
    static size_t cache_cap = 0;     /* allocated capacity */
    static size_t cache_len = 0;     /* number of cached primes */
    static idx_t   next_k   = 0;     /* next integer k to test */

    /* Generate primes until we have at least n of them. */
    while (cache_len < n) {
        /* 210 = 2·3·5·7 */
        seq_t candidate = (seq_t)210 * (seq_t)next_k + (seq_t)83;

        if (is_prime_big(candidate)) {
            /* Ensure there is space for the new prime. */
            if (cache_len + 1 > cache_cap) {
                /* grow_seq will increase the capacity to at least `need`. */
                grow_seq(&cache, &cache_cap, cache_len + 1);
            }
            cache[cache_len++] = candidate;
        }
        next_k++;  /* proceed to the next k regardless of primality */
    }

    /* cache[n‑1] holds the nth term (1‑based index). */
    return cache[n - 1];
}
