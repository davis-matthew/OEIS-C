/*
 * A216782  Numbers such that numerator(sigma(n)/n) is even and
 *          denominator(sigma(n)/n) is odd.
 * https://oeis.org/A216782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth positive integer k for which σ(k)/k in lowest * terms
 *            has an even numerator and an odd denominator
 * Range:     u8 n <= 178, u16 n <= 8000, u32 n <= 8000, u64 n <= 8000,
 *            u128 n <= 8000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Count trailing zeros of a 64‑bit integer (used for exponents). */
static unsigned trailing_zeros_uint64(uint64_t x) {
    unsigned c = 0;
    while ((x & 1ULL) == 0ULL) {
        ++c;
        x >>= 1;
    }
    return c;
}

/* Compute v₂(σ(m)) for odd m as the sum over its prime powers.
 * For pᵉ with e odd: contribution = v₂(p + 1) + v₂(e + 1) – 1.
 * (For e even the contribution is zero.) */
static unsigned calc_v2_sigma_odd(uint64_t m) {
    unsigned total = 0;
    uint64_t temp = m;

    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            unsigned e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            if (e & 1) {                     /* e odd */
                unsigned vp = trailing_zeros_uint64(p + 1);
                unsigned ve = trailing_zeros_uint64(e + 1);
                total += vp + ve - 1;
            }
        }
    }

    if (temp > 1) {                         /* remaining prime factor */
        /* exponent e = 1 (odd) */
        unsigned vp = trailing_zeros_uint64(temp + 1);
        total += vp;                         /* +1−1 simplifies to 0 */
    }

    return total;
}

/* Test whether n satisfies the sequence condition. */
static bool is_good(uint64_t n) {
    unsigned a = 0;                         /* v₂(n) */
    while ((n & 1ULL) == 0ULL) {
        ++a;
        n >>= 1;
    }
    uint64_t m = n;                         /* odd part */

    unsigned t = calc_v2_sigma_odd(m);       /* v₂(σ(m)) */
    return t > a;                           /* strict inequality */
}

/* Dynamic array to store discovered sequence terms. */
static seq_t *term_buf   = NULL;
static size_t term_cap   = 0;
static size_t term_cnt   = 0;
static uint64_t last_num = 0;

/* Sequentially generate terms up to the desired index. */
seq_t a(idx_t n) {
    if (n == 0) return 0;                   /* offset starts at 1 */

    /* Allocate buffer once. */
    if (!term_buf) {
        term_cap = 128;
        term_buf = (seq_t *)malloc(term_cap * sizeof(seq_t));
        if (!term_buf) abort();
    }

    /* Return previously computed term if available. */
    if ((size_t)n <= term_cnt)
        return term_buf[n - 1];

    /* Extend the list until we reach the required index. */
    while (term_cnt < n) {
        ++last_num;
        if (is_good(last_num)) {
            if (term_cnt == term_cap) {
                term_cap <<= 1;
                term_buf = (seq_t *)realloc(term_buf, term_cap * sizeof(seq_t));
                if (!term_buf) abort();
            }
            term_buf[term_cnt++] = (seq_t)last_num;
        }
    }

    return term_buf[n - 1];
}
