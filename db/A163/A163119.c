/*
 * A163119  Absolute values of the first differences in the digit chain of the
 *          concatenated primes.
 * https://oeis.org/A163119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = |A033308(n) - A033308(n-1)|
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A163119 */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Count decimal digits of a 64‑bit integer */
static int num_digits(uint64_t x) {
    int d = 0;
    do {
        d++;
        x /= 10;
    } while (x);
    return d;
}

/* Global state for primes and cumulative digit counts */
static uint64_t *g_primes = NULL;
static uint64_t *g_cum   = NULL;
static size_t   g_cap   = 0;
static size_t   g_len   = 0;
static uint64_t g_last_checked = 1;   /* last integer tested for primality */

/* Append a new prime to the global tables */
static void add_prime(uint64_t p) {
    /* Grow memory if needed */
    if (g_len == g_cap) {
        size_t new_cap = g_cap ? g_cap * 2 : 8;
        uint64_t *tmp = (uint64_t *)realloc(g_primes, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        g_primes = tmp;
        tmp = (uint64_t *)realloc(g_cum, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        g_cum = tmp;
        g_cap = new_cap;
    }

    g_primes[g_len] = p;
    int d = num_digits(p);
    g_cum[g_len] = (g_len == 0) ? (uint64_t)d : g_cum[g_len - 1] + (uint64_t)d;
    g_len++;
    g_last_checked = p;
}

/* Simple primality test using already known primes */
static bool is_prime_candidate(uint64_t cand) {
    if (cand < 2) return false;
    if (cand % 2 == 0) return cand == 2;
    for (size_t i = 0; i < g_len &&
                     ((uint64_t)g_primes[i] * g_primes[i] <= cand); ++i) {
        if (cand % g_primes[i] == 0) return false;
    }
    return true;
}

/* Ensure at least 'need' digits of the concatenated primes are generated */
static void ensure_digits(uint64_t need) {
    if (g_len == 0) {
        add_prime(2);
    }
    uint64_t cand = g_last_checked + 1;
    while (g_cum[g_len - 1] < need) {
        if (is_prime_candidate(cand)) {
            add_prime(cand);
        }
        cand++;
    }
}

/* Return the digit at 1‑based position 'pos' in the concatenated prime chain */
static int digit_at_position(uint64_t pos) {
    /* Binary search on cumulative counts */
    size_t lo = 0, hi = g_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (g_cum[mid] >= pos)
            hi = mid;
        else
            lo = mid + 1;
    }
    size_t idx = lo;
    uint64_t prime = g_primes[idx];
    uint64_t prev = (idx == 0) ? 0 : g_cum[idx - 1];
    uint64_t offset = pos - prev - 1; /* 0‑based offset within the prime */
    int len = num_digits(prime);
    /* Extract digits of the prime into an array */
    unsigned char digs[20];
    uint64_t tmp = prime;
    for (int i = len - 1; i >= 0; --i) {
        digs[i] = tmp % 10;
        tmp /= 10;
    }
    return digs[(size_t)offset];
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 1) return 0;
    uint64_t pos1 = n;
    uint64_t pos2 = n + 1;
    ensure_digits(pos2);
    int d1 = digit_at_position(pos1);
    int d2 = digit_at_position(pos2);
    int diff = (d1 > d2) ? d1 - d2 : d2 - d1;
    return (seq_t)diff;
}
