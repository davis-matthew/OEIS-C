/*
 * A175785  Numbers n such that the number of distinct prime divisors of n
 *          does not divide phi(n).
 * https://oeis.org/A175785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 6835, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9278 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A175785: all integers n such that ω(n) does not divide φ(n).
   The function a(n) returns the nth element of this sequence (offset 1). */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

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

/* Compute Euler's totient φ of an odd 64‑bit or larger number m.
   The algorithm uses trial division up to sqrt(m). */
static seq_t calc_phi(seq_t m)
{
    seq_t result = m;
    seq_t p = 2;

    /* Handle factor 2 separately to skip it later (m is odd, but keep robust) */
    if (m % 2 == 0) {
        while (m % 2 == 0)
            m /= 2;
        result -= result / 2;
    }

    /* Trial division by odd primes */
    for (p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            while (m % p == 0)
                m /= p;
            result -= result / p;
        }
    }

    /* If m > 1, it is a prime factor larger than sqrt(original m) */
    if (m > 1)
        result -= result / m;

    return result;
}

/* Count distinct prime factors of an unsigned 64-bit integer n.
   The result fits in a signed int for all n that may be passed. */
static int count_distinct_primes(uint64_t n)
{
    if (n <= 1)
        return 0;

    int cnt = 0;
    /* Handle factor 2 separately. */
    if (n % 2 == 0) {
        cnt++;
        while (n % 2 == 0)
            n /= 2;
    }
    /* Test odd divisors up to sqrt(n). */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            cnt++;
            while (n % p == 0)
                n /= p;
        }
    }
    if (n > 1)   /* Remaining prime factor. */
        cnt++;
    return cnt;
}

/* External linkage as requested */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;         /* stored sequence terms */
    static size_t cache_size = 0;       /* number of terms stored */
    static size_t cache_cap = 0;        /* capacity of cache array */
    static idx_t last_checked = 1;      /* last integer examined */

    while (cache_size < n) {
        last_checked++;
        /* Compute the number of distinct prime factors (ω) */
        int omega = count_distinct_primes((uint64_t)last_checked);
        if (omega == 0) /* skip n = 1 */
            continue;
        /* Compute φ(n) */
        seq_t phi_val = calc_phi((uint64_t)last_checked);

        /* If ω does not divide φ, store the number */
        if ((phi_val % omega) != 0) {
            grow_seq(&cache, &cache_cap, cache_size + 1);
            cache[cache_size++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
