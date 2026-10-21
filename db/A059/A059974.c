/*
 * A059974  a(n)=a(p)+a(q) where p and q are the two primes less than n and
 *          closest to n; with a(1)=1, a(2)=1, a(3) = 1.
 * https://oeis.org/A059974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(p) + a(q) where p and q are the two primes less than n
 *            that are closest to n; a(1)=1, a(2)=1, a(3)=1
 * Range:     u8 n <= 41, u16 n <= 89, u32 n <= 211, u64 n <= 487,
 *            u128 n <= 1109, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- Global mutable state ---------- */

/* Cache of already computed a(n) values (0‑based index) */
static seq_t *a_cache = NULL;
static size_t a_cache_size = 0;          /* allocated size of a_cache       */
static size_t a_computed_up_to = 0;      /* largest n that has been computed */

/* List of primes in ascending order */
static uint32_t *prime_list = NULL;
static size_t prime_cap   = 0;
static size_t prime_len   = 0;

/* The largest integer that has been tested for primality */
static idx_t last_checked = 1;

/* ---------- Helpers ---------- */

static void ensure_seq_capacity(seq_t **arr, size_t *size, size_t need)
{
    if (need <= *size)
        return;
    size_t new_cap = (*size == 0) ? 64 : *size * 2;
    while (new_cap < need)
        new_cap <<= 1;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *size = new_cap;
}

static void ensure_prime_capacity(size_t need)
{
    if (need <= prime_cap)
        return;
    size_t new_cap = (prime_cap == 0) ? 64 : prime_cap * 2;
    while (new_cap < need)
        new_cap <<= 1;
    prime_list = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
    prime_cap = new_cap;
}

/* Trial division primality test using the primes we already know */
static bool is_prime(idx_t n)
{
    if (n < 2)
        return false;
    for (size_t i = 0; i < prime_len; ++i) {
        uint32_t p = prime_list[i];
        /* avoid overflow: stop if p > n/p */
        if ((idx_t)p > n / (idx_t)p)
            break;
        if (n % (idx_t)p == 0)
            return false;
    }
    return true;
}

/* Extend the prime list up to and including lim */
static void ensure_primes_up_to(idx_t lim)
{
    for (idx_t x = last_checked + 1; x <= lim; ++x) {
        if (is_prime(x)) {
            ensure_prime_capacity(prime_len + 1);
            prime_list[prime_len++] = (uint32_t)x;
        }
    }
    last_checked = lim;
}

/* Return the index of the first prime >= val in prime_list [0..prime_len) */
static size_t first_prime_ge(idx_t val)
{
    size_t lo = 0, hi = prime_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if ((idx_t)prime_list[mid] >= val)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;                       /* may equal prime_len if all primes < val */
}

/* Return the two primes less than x and closest to x (x > 3 guarantee) */
static void two_largest_primes_less_than(idx_t x, idx_t *p, idx_t *q)
{
    size_t idx = first_prime_ge(x);
    /* idx is first prime >= x; we need the two primes before it */
    idx_t p_idx = (idx_t)(idx - 1);
    idx_t q_idx = (idx_t)(idx - 2);
    *p = (idx_t)prime_list[p_idx];
    *q = (idx_t)prime_list[q_idx];
}

/* ---------- Public function ---------- */

seq_t a(idx_t n)
{
    if (n == 0)          /* safety; sequence starts at 1 */
        return (seq_t)0;

    /* If already computed, return cached value */
    if ((size_t)n <= a_computed_up_to)
        return a_cache[(size_t)n - 1];

    /* Ensure we have primes up to n-1 (needed for all indices up to n) */
    if (last_checked < n - 1)
        ensure_primes_up_to(n - 1);

    /* Ensure cache capacity */
    ensure_seq_capacity(&a_cache, &a_cache_size, (size_t)n);

    /* Compute values from the next not-yet-computed index up to n */
    for (idx_t idx = a_computed_up_to + 1; idx <= n; ++idx) {
        if (idx <= 3) {
            a_cache[(size_t)idx - 1] = (seq_t)1;
        } else {
            idx_t p, q;
            two_largest_primes_less_than(idx, &p, &q);
            /* indices in cache are 0‑based */
            a_cache[(size_t)idx - 1] =
                a_cache[(size_t)p - 1] + a_cache[(size_t)q - 1];
        }
    }

    a_computed_up_to = (size_t)n;
    return a_cache[(size_t)n - 1];
}
