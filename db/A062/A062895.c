/*
 * A062895  Numbers k for which d(k) = d(R(k)), where R(k) is the reversal of
 *          k and d(k) is the number of divisors of k.
 * https://oeis.org/A062895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A062895: a(n) = the nth integer k such that d(k) = d(R(k)),
   where d(k) is the number of positive divisors of k and R(k) is
   the decimal reversal of k.  The function is lazy and memoises
   results in a dynamically‑grown table.  It uses an intrinsic
   divisor‑count routine; no external helper function is required. */

#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Helper: reverse a 64‑bit integer in decimal.                       */
/* ------------------------------------------------------------------ */
static inline uint64_t rev_uint64(uint64_t x)
{
    uint64_t r = 0;
    while (x > 0) {
        r = r * 10 + (x % 10);
        x /= 10;
    }
    return r;
}

/* ------------------------------------------------------------------ */
/* Helper: number of positive divisors of a 64‑bit integer.            */
/* ------------------------------------------------------------------ */
static inline uint64_t divisor_count_u64(uint64_t n)
{
    uint64_t cnt = 1;
    uint64_t e, m = n;

    /* factor 2 */
    e = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        ++e;
    }
    if (e > 0) cnt *= (e + 1);

    /* odd factors */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            cnt *= (e + 1);
        }
    }

    if (m > 1)          /* remaining prime factor */
        cnt *= 2;

    return cnt;
}

/* ------------------------------------------------------------------ */
/* Memoised table for the generated terms.                             */
/* ------------------------------------------------------------------ */
static seq_t *terms  = NULL;
static size_t  terms_len = 0;   /* number of stored terms */
static size_t  terms_cap = 0;   /* capacity of the table       */
static uint64_t next_candidate = 1;  /* next integer to test */

static void grow_terms(void)
{
    if (terms_cap == 0) {
        terms_cap = 128;
        terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
    } else if (terms_len + 1 > terms_cap) {
        terms_cap <<= 1;
        terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
    }
}

static void add_term(seq_t v)
{
    grow_terms();
    terms[terms_len++] = v;
}

/* Generate terms until we have at least n of them. */
static void generate_up_to(idx_t n)
{
    /* idx_t is unsigned; ensure it can be cast to size_t safely. */
    while ((idx_t)terms_len < n) {
        uint64_t cand = next_candidate;
        uint64_t rev = rev_uint64(cand);
        if (divisor_count_u64(cand) == divisor_count_u64(rev))
            add_term((seq_t)cand);
        ++next_candidate;
    }
}

/* Public entry point.  n must be ≥ 1 for a correct OEIS offset. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* safety for an accidental 0 index */
    generate_up_to(n);
    return terms[n - 1];
}
