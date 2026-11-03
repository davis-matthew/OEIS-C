/*
 * A050758  Primes containing at least one pair of consecutive equal digits.
 * https://oeis.org/A050758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th prime p that contains at least one pair of
 *            consecutive equal digits
 * Range:     u8 n <= 8, u16 n <= 2121, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9760 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function generates primes in increasing order, keeps only those with
   repeated adjacent digits, and returns the requested term. */

/* External utilities provided by the build environment */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

/* Test whether a seq_t value has any pair of consecutive equal decimal digits. */
static inline bool no_consecutive_equal_digits(seq_t val) {
    if (val == (seq_t)0) return true;          /* 0 contains only one digit */
    int prev = -1;                             /* previous digit, -1 means none yet */
    seq_t tmp = val;
    while (tmp > (seq_t)0) {
        int d = (int)(tmp % (seq_t)10);        /* current least‑significant digit */
        if (prev == d) return false;           /* found equal neighbours */
        prev = d;
        tmp /= (seq_t)10;                      /* shift right one decimal digit */
    }
    return true;                               /* all adjacent digits differ */
}

static seq_t *g_terms      = NULL;  /* stored primes with consecutive equal digits */
static size_t g_terms_len  = 0;     /* number of stored terms */
static size_t g_terms_cap  = 0;     /* allocated capacity */
static seq_t  g_candidate  = 0;     /* next odd prime candidate to test */
static bool   g_initialized = false;

/* Ensure that at least n terms are available */
static void ensure_terms(idx_t n)
{
    if (!g_initialized) {
        g_terms_cap   = 16;
        g_terms       = mem_new(seq_t, g_terms_cap);
        g_candidate   = (seq_t)11;          /* start with first odd prime > 10 */
        g_initialized = true;
    }

    if (!g_terms) return;

    while (g_terms_len < (size_t)n) {
        /* Test current candidate */
        if (is_prime_big(g_candidate) && !no_consecutive_equal_digits(g_candidate)) {
            /* Grow the array if necessary */
            if (g_terms_len >= g_terms_cap) {
                g_terms_cap <<= 1;
                g_terms = mem_resize(seq_t, g_terms, g_terms_cap);
            }
            g_terms[g_terms_len++] = g_candidate;
        }

        /* Advance to the next odd candidate */
        g_candidate += (seq_t)2;

        /* Avoid infinite loop when seq_t overflows (e.g., 8‑bit builds) */
        if (g_candidate < (seq_t)3) break;
    }
}

/* Return the n‑th term (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;     /* undefined for n = 0 */
    ensure_terms(n);
    return g_terms[(size_t)(n - 1)];        /* zero‑based indexing inside array */
}
