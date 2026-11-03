/*
 * A214069  Least m>0 such that prime(n)+m and n-m are relatively prime.
 * https://oeis.org/A214069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = min{ m>0 : gcd(prime(n)+m, |n-m|) = 1 }
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 115 terms through n = 843 (2026-11-01)
 * Helpers:   gcd, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/ipow_i32.h"

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

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}

/* Static prime table used to obtain prime(n). */
static uint32_t *primes = NULL;      /* array of primes */
static size_t   primes_cnt = 0;      /* number of primes stored */
static size_t   primes_cap = 0;      /* capacity of the primes array */
static uint32_t last_checked = 1;    /* last tested integer */

/* Append a new prime to the static table. */
static void append_prime(uint32_t p)
{
    if (primes_cnt + 1 > primes_cap)
        ensure_capacity(primes_cnt + 1, &primes_cap,
                        (void**)&primes, sizeof(uint32_t));
    primes[primes_cnt++] = p;
    last_checked = p;
}

/* Generate primes until we have at least 'target' primes. */
static void grow_primes_until(idx_t target)
{
    if (primes_cnt >= (size_t)target)
        return;

    for (uint32_t cand = last_checked + 1; ; ++cand)
    {
        /* Trial division by previously stored primes. */
        uint32_t lim = (uint32_t)floor_sqrt((seq_t)cand);
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i)
        {
            uint32_t p = primes[i];
            if (p > lim) break;
            if (cand % p == 0) { is_prime = false; break; }
        }

        if (is_prime)
        {
            append_prime(cand);
            if (primes_cnt == (size_t)target)
                return;
        }
    }
}

/* Return the n-th term of A214069.  n ≥ 1. */
seq_t a(idx_t n)
{
    /* Ensure we have the n-th prime. */
    grow_primes_until(n);
    uint32_t p = primes[n - 1];         /* prime(n) */

    for (idx_t m = 1; ; ++m)
    {
        /* Difference |n-m| – treat zero as “ignore”. */
        idx_t diff = (n > m) ? (n - m) : (m - n);
        if (diff == 0)
            continue;

        seq_t a_val = (seq_t)p + (seq_t)m;
        seq_t b_val = (seq_t)diff;

        if (gcd(a_val, b_val) == (seq_t)1)
            return (seq_t)m;
    }

    /* Unreachable – the loop always returns. */
    return (seq_t)0;
}
