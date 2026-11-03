/*
 * A175762  Primes with an arithmetic mean of digits which is also prime.
 * https://oeis.org/A175762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 740, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 963 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Implements the OEIS formula:
 *   a(n) = the nth prime p such that (sum of decimal digits of p)
 *           divided by (number of decimal digits of p) is an integer
 *           prime.
 *
 * The function enumerates primes in increasing order and tests each
 * candidate until it has found n qualifying primes.  Results are cached
 * in a static array so that calls with arbitrary n order are fast.
 */
#include <stdlib.h>
#include <stdbool.h>

/* Utility functions from the build */
#include <stdint.h>
#include "../utils/ipow_i32.h"
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

static seq_t *term_list = NULL;   /* cached results */
static size_t term_len  = 0;      /* number of cached terms */
static size_t term_cap  = 0;      /* allocated capacity */

static seq_t last_checked = 1;    /* last integer examined */

/* Expand the cached term array lazily */
static void grow_term_list(void)
{
    size_t new_cap = (term_cap == 0) ? 8 : term_cap << 1;
    seq_t *tmp = mem_resize(seq_t, term_list, new_cap);
    if (!tmp) exit(EXIT_FAILURE);
    term_list = tmp;
    term_cap  = new_cap;
}

/* Append a new term to the cache */
static void add_term(seq_t val)
{
    if (term_len == term_cap) grow_term_list();
    term_list[term_len++] = val;
}

/* Count decimal digits of a number */
static size_t digit_count(seq_t x)
{
    if (x == 0) return 1;
    size_t d = 0;
    seq_t v = x;
    while (v > 0) {
        v /= 10;
        ++d;
    }
    return d;
}

/* Sum decimal digits of a number */
static seq_t digit_sum(seq_t x)
{
    seq_t sum = 0;
    seq_t v = x;
    while (v > 0) {
        sum += v % 10;
        v /= 10;
    }
    return sum;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range, but safe return */

    /* Initialise static storage on first call */
    if (!term_list) {
        term_cap = 8;
        term_list = mem_new(seq_t, term_cap);
        term_len  = 0;
    }

    /* Generate terms until we have at least n of them */
    while (term_len < n) {
        /* Find next prime candidate after last_checked */
        seq_t cand = last_checked + 1;
        /* For efficiency skip even numbers */
        if (cand > 2 && (cand & 1) == 0) ++cand;

        for (;;) {
            if (is_prime_big(cand)) {
                /* Evaluate the digit mean condition */
                seq_t sum = digit_sum(cand);
                size_t d   = digit_count(cand);
                if ((seq_t)d != 0 && (sum % (seq_t)d) == 0) {
                    seq_t mean = sum / (seq_t)d;
                    if (is_prime_big(mean)) {
                        add_term(cand);
                    }
                }
                last_checked = cand;   /* next search starts after this */
                break;               /* go to next candidate */
            }
            cand += 2;               /* skip evens */
        }
    }

    return term_list[n - 1];         /* index offset 1-based */
}
