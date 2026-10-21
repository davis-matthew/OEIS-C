/*
 * A084109  n is congruent to 1 (mod 4) and is not the sum of two squares.
 * https://oeis.org/A084109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A084109 – n ≡ 1 (mod 4) and not a sum of two squares.
    In code: the nth such integer is generated.
    A number is not a sum of two squares iff it has an odd exponent
    for at least one prime ≡ 3 (mod 4).  For n ≡ 1 (mod 4) the
    count of distinct primes ≡ 3 (mod 4) with odd exponent must be
    at least 2.  The implementation builds a sieve for smallest
    prime factors, then scans consecutive numbers ≡ 1 (mod 4)
    until enough terms have been collected.  It automatically
    enlarges the sieve if the requested n exceeds the current
    range.  The algorithm runs in O(N) time up to the bounding
    value (≈ 200 000 for 10 000 terms).  All computations stay
    within 32‑bit arithmetic because the found terms fit well
    inside a 32‑bit integer.  Intermediates are cast to seq_t
    only when storing the final answer.  */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Helper: build sieve of smallest prime factor up to 'limit'         */
/* ------------------------------------------------------------------ */
static uint32_t *spfs = NULL;
static uint32_t  sieve_limit = 0;

static void build_spf(uint32_t limit) {
    if (limit <= sieve_limit) return;                 /* already built */

    if (spfs) free(spfs);
    spfs = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
    for (uint32_t i = 2; i * i <= limit; ++i) {
        if (spfs[i] == 0) {
            spfs[i] = i;
            for (uint32_t j = i * i; j <= limit; j += i)
                if (spfs[j] == 0) spfs[j] = i;
        }
    }
    for (uint32_t i = 2; i <= limit; ++i)
        if (spfs[i] == 0) spfs[i] = i;
    sieve_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Test whether a number num (fits in uint32_t) satisfies the
   conditions of the sequence.                                          */
/* ------------------------------------------------------------------ */
static bool qualifies(uint32_t num) {
    if (num % 4 != 1) return false;
    int cnt = 0;
    uint32_t x = num;
    while (x > 1) {
        uint32_t p = spfs[x];
        int exp = 0;
        while (x % p == 0) {
            x /= p;
            exp++;
        }
        if ((p % 4) == 3 && (exp & 1)) {
            cnt++;
            if (cnt >= 2) return true;
        }
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Dynamically build the list of sequence terms up to 'need' terms.   */
/* ------------------------------------------------------------------ */
static seq_t *terms = NULL;
static size_t  terms_size = 0;
static size_t  terms_cap  = 0;
static uint32_t last_checked = 0;

static void grow_terms(size_t new_cap) {
    if (new_cap <= terms_cap) return;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms = tmp;
    terms_cap = new_cap;
}

/* Generate numbers from 'st' to 'end' (inclusive) and append
   qualifying ones.                                                    */
static void collect_terms(uint32_t st, uint32_t end) {
    for (uint32_t n = st; n <= end; ++n) {
        if (qualifies(n)) {
            if (terms_size == terms_cap)
                grow_terms(terms_cap ? terms_cap * 2 : 64);
            terms[terms_size++] = (seq_t)n;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Main routine called by the test harness.                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    /* Ensure at least 'n' terms are available.  Build incrementally. */
    if (n == 0) return (seq_t)0;          /* undefined for 0 but safe */

    /* Initial bound is generous enough for n up to 10 000. */
    uint32_t needed_limit = 200000;
    if (!terms) {
        grow_terms(64);
        build_spf(needed_limit);
        collect_terms(5, needed_limit);   /* first number ≡1 mod4 is 5 */
        last_checked = needed_limit;
    }

    /* If more terms are required, enlarge the sieve and continue. */
    while (terms_size < (size_t)n) {
        uint32_t new_limit = last_checked * 2;
        build_spf(new_limit);
        collect_terms(last_checked + 1, new_limit);
        last_checked = new_limit;
    }

    return terms[n - 1];
}
