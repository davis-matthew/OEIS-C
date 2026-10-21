/*
 * A072202  Same numbers of prime factors of forms 4*k+1 and 4*k+3, counted
 *          with multiplicity.
 * https://oeis.org/A072202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A072202: sequence of integers n such that the numbers of prime factors
   ≡1 (mod 4) and ≡3 (mod 4) (counted with multiplicity) are equal. */

/* Standard headers.  The build defines the types idx_t and seq_t. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* ---------- Static storage for memoisation ---------- */
static seq_t *terms      = NULL;  /* sequence values, 1‑based index   */
static size_t terms_cap  = 0;
static size_t terms_len  = 0;      /* number of terms stored (≥1)       */

static uint32_t *primes   = NULL;  /* list of primes, 2,3,5,…          */
static size_t primes_cap  = 0;
static size_t primes_len  = 0;
static uint32_t primes_last_checked = 0; /* next odd number to test */

/* next integer to be examined (>=2) */
static uint32_t next_candidate = 2;

/* ---------- Helper utilities ---------- */

/* Ensure the primes array contains all primes ≤ limit (inclusive).
   The array is grown as necessary. */
static void ensure_primes(uint32_t limit) {
    if (limit < 2) return;
    if (limit < primes_last_checked) return;          /* already covered */

    /* Start from the last tested odd number.  If we haven't tested any
       odd yet, start from 3. */
    uint32_t cand = (primes_last_checked > 2) ? primes_last_checked : 3;

    for (; cand <= limit; cand += 2) {
        bool composite = false;
        for (size_t i = 0; i < primes_len &&
                 (uint64_t)primes[i] * primes[i] <= cand; ++i) {
            if (cand % primes[i] == 0) {
                composite = true;
                break;
            }
        }
        if (!composite) {
            if (primes_len == primes_cap) {
                primes_cap = (primes_cap ? primes_cap * 2 : 8);
                primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_len++] = cand;
        }
    }
    primes_last_checked = limit + 2;   /* next odd > limit */
}

/* Ensure the terms array has capacity for at least 'needed' elements. */
static void ensure_terms_capacity(size_t needed) {
    if (needed <= terms_cap) return;
    size_t new_cap = (terms_cap ? terms_cap * 2 : 32);
    while (new_cap < needed) new_cap <<= 1;
    terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}

/* Initialise static data on first use. */
static void init_static(void) {
    if (terms) return;
    terms_cap = 32;
    terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    terms[0] = 0;                  /* unused in 1‑based indexing */
    terms[1] = (seq_t)1;           /* first term of the sequence */
    terms_len = 1;

    primes_cap = 8;
    primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    primes[0] = 2;
    primes_len = 1;
    primes_last_checked = 3;        /* next odd to test */
    next_candidate = 2;
}

/* Determine whether the integer v satisfies the A072202 condition. */
static bool qualifies(uint32_t v) {
    if (v == 1) return true;   /* 1 has no prime factors */

    int cnt1 = 0, cnt3 = 0;
    uint32_t tmp = v;

    /* Ensure we have all primes up to sqrt(tmp) for factorisation. */
    uint32_t sqrt_lim = (uint32_t) sqrt((double)tmp);
    ensure_primes(sqrt_lim);

    for (size_t i = 0; i < primes_len &&
         (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            if (p % 4 == 1) cnt1 += e;
            else if (p % 4 == 3) cnt3 += e;
        }
    }

    /* If a prime larger than sqrt(tmp) remains. */
    if (tmp > 1) {
        if (tmp % 4 == 1) ++cnt1;
        else if (tmp % 4 == 3) ++cnt3;
    }

    return cnt1 == cnt3;
}

/* ---------- Public sequence function ---------- */

seq_t a(idx_t n) {
    init_static();

    if (n <= terms_len) return terms[n];

    /* Generate further terms until index n is reached. */
    while (terms_len < (size_t)n) {
        uint32_t v = next_candidate++;
        if (qualifies(v)) {
            ensure_terms_capacity(terms_len + 2);
            terms[++terms_len] = (seq_t)v;
        }
    }
    return terms[n];
}
