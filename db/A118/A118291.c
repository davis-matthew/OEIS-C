/*
 * A118291  a(1) = 1. a(n) = number of terms among the sequence's first (n-1)
 *          terms which are divisible by the largest prime dividing a(n-1), or
 *          which are divisible by 1 if a(n-1)= 1.
 * https://oeis.org/A118291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>1, a(n)=#terms among the first n‑1 terms that are
 *            divisible by the largest prime factor of a(n-1), * or n-1 if
 *            a(n-1)=1
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_N 10000               /* largest n expected in the specification */
#define MAX_VAL (MAX_N)           /* a(n) <= n-1 <= MAX_N */
#define SPF_LIMIT (MAX_VAL)

/* static arrays */
static uint64_t spf_arr[SPF_LIMIT + 1];
static bool spf_built = false;
static unsigned short countDiv[MAX_VAL + 1];   /* counts of terms divisible by prime p */
static seq_t *seq_arr = NULL;
static size_t seq_cap = 0;
static idx_t seq_computed_until = 0;

/* Build smallest prime factor array up to SPF_LIMIT. */
static void build_spf(void) {
    if (spf_built) return;
    for (size_t i = 0; i <= SPF_LIMIT; ++i) spf_arr[i] = i;
    for (size_t p = 2; p * p <= SPF_LIMIT; ++p) {
        if (spf_arr[p] == p) {
            for (size_t j = p * p; j <= SPF_LIMIT; j += p)
                if (spf_arr[j] == j) spf_arr[j] = p;
        }
    }
    spf_built = true;
}

/* Return largest prime divisor of m (assumes m <= SPF_LIMIT). If m==1 returns 1. */
static uint64_t largest_prime_div(uint64_t m) {
    if (m <= 1) return 1;
    uint64_t maxp = 1;
    while (m > 1) {
        uint64_t p = spf_arr[(size_t)m];
        if (p > maxp) maxp = p;
        while (m % p == 0) m /= p;
    }
    return maxp;
}

/* Ensure seq_arr has capacity for at least new_cap elements (1-indexed). */
static void ensure_seq_capacity(size_t new_cap) {
    if (seq_cap >= new_cap) return;
    size_t newc = seq_cap > 0 ? seq_cap * 2 : new_cap;
    if (newc < new_cap) newc = new_cap;
    seq_t *tmp = (seq_t*)realloc(seq_arr, newc * sizeof(seq_t));
    if (!tmp) abort();
    seq_arr = tmp;
    seq_cap = newc;
}

/* Main function to compute a(n). */
seq_t a(idx_t n) {
    if (n < 1) return 0;          /* Defined only for n >= 1 */
    build_spf();
    ensure_seq_capacity((size_t)n + 1); /* 1-indexed */

    /* Initialize first term if not done yet. */
    if (seq_computed_until == 0) {
        seq_arr[1] = (seq_t)1;
        seq_computed_until = 1;
    }

    /* Compute terms up to n if necessary. */
    for (idx_t i = seq_computed_until + 1; i <= n; ++i) {
        seq_t prev = seq_arr[(size_t)i - 1];
        seq_t val;
        if ((uint64_t)prev == 1) {      /* a(n-1) == 1 -> a(n) = n-1 */
            val = (seq_t)(i - 1);
        } else {
            uint64_t p = largest_prime_div((uint64_t)prev);
            val = (seq_t)countDiv[p];
        }
        seq_arr[(size_t)i] = val;

        /* Update countDiv for term val (if >1). */
        if (val > 1) {
            uint64_t v = (uint64_t)val;
            while (v > 1) {
                uint64_t p = spf_arr[(size_t)v];
                countDiv[p]++;       /* increment count for this prime */
                while (v % p == 0) v /= p;
            }
        }
    }
    seq_computed_until = n;
    return seq_arr[(size_t)n];
}
