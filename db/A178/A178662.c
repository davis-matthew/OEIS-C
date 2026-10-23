/*
 * A178662  Semiprimes of the form m*k such that (m-1)/(k+1) is also
 *          semiprime.
 * https://oeis.org/A178662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search of integers and test the
   semiprime condition described in A178662.
   No closed formula exists, so the routine searches successive
   integers and keeps those that satisfy the definition. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Helper for integer square root (no math.h) ---------- */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = n, mid, res = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        if (mid <= n / mid) {
            res = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return res;
}

/* ---------- Prime storage and construction ---------- */
static uint64_t *prime_list   = NULL;
static size_t  prime_cnt      = 0;
static size_t  prime_cap      = 0;
static uint64_t last_checked  = 1U;          /* highest number checked for primality */

/* add prime to list, resizing as necessary */
static void append_prime(uint64_t p)
{
    if (prime_cnt == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 32;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) exit(1);
        prime_list = tmp;
        prime_cap  = new_cap;
    }
    prime_list[prime_cnt++] = p;
}

/* extend prime list up to given limit (inclusive) */
static void ensure_primes_up_to(uint64_t limit)
{
    if (limit <= last_checked) return;

    for (uint64_t num = last_checked + 1; num <= limit; ++num) {
        bool is_p = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > num) break;
            if (num % p == 0) { is_p = false; break; }
        }
        if (is_p) append_prime(num);
    }
    last_checked = limit;
}

/* primality test using current prime list */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    uint64_t limit = isqrt_u64(n);
    ensure_primes_up_to(limit);
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* check if x is a semiprime and return its two prime factors
   in order: *p_big >= *p_small.  Return true on success. */
static bool semiprime_factors(uint64_t x, uint64_t *p_big, uint64_t *p_small)
{
    if (x < 4) return false;          /* 2*2 is the smallest semiprime */

    uint64_t limit = isqrt_u64(x);
    ensure_primes_up_to(limit);

    uint64_t p = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t prime = prime_list[i];
        if (prime * prime > x) break;
        if (x % prime == 0) { p = prime; break; }
    }
    if (!p) return false;              /* prime number */

    uint64_t q = x / p;
    if (q <= 1) return false;          /* shouldn't happen */

    if (!is_prime_uint64(q)) return false;   /* more than two prime factors */

    if (p >= q) { *p_big = p; *p_small = q; }
    else        { *p_big = q; *p_small = p; }
    return true;
}

/* ---------- Storage for computed sequence ---------- */
static seq_t *seq_vals      = NULL;
static size_t seq_len       = 0;
static size_t seq_cap       = 0;
static uint64_t next_candidate = 4U;   /* next integer to test */

/* push a new term into the sequence array */
static void append_value(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 32;
        seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        seq_vals = tmp;
        seq_cap  = new_cap;
    }
    seq_vals[seq_len++] = val;
}

/* ---------- Main sequence routine ---------- */
seq_t a(idx_t n)
{
    if (seq_len >= (size_t)n) return seq_vals[n - 1];

    /* Main loop: generate terms until we have at least n values */
    while (seq_len < (size_t)n) {
        uint64_t x = next_candidate++;

        /* Is x a semiprime? */
        uint64_t m, k;
        if (!semiprime_factors(x, &m, &k)) continue;  /* not a semiprime */

        /* Check (m-1)/(k+1) semiprime */
        uint64_t denom = k + 1U;
        if ((m - 1U) % denom != 0) continue;
        uint64_t q = (m - 1U) / denom;
        if (q <= 1) continue;
        uint64_t q_big, q_small;
        if (!semiprime_factors(q, &q_big, &q_small)) continue;

        /* Success: store term */
        append_value((seq_t)x);
    }

    return seq_vals[n - 1];
}
