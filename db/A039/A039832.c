/*
 * A039832  Numbers k such that k and k+1 both have 4 divisors.
 * https://oeis.org/A039832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 2189, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth integer k such that both
 * k and k+1 have exactly 4 positive divisors.
 * It computes terms by searching for consecutive numbers
 * each having 4 divisors, using a small‑prime‑factor sieve.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- Prime‑factor sieve (smallest prime factor) ---------- */

static uint32_t *spf = NULL;   /* smallest prime factor array */
static size_t   spf_limit = 0; /* current maximum index in spf */

/* Rebuild the sieve up to 'limit' (inclusive).  Free previous array. */
static void recompute_spf(size_t limit)
{
    free(spf);
    spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!spf) exit(1);     /* allocation failed */

    /* init to zero */
    for (size_t i = 0; i <= limit; ++i) spf[i] = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                 /* i is prime */
            spf[i] = (uint32_t)i;
            if ((uint64_t)i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
    spf_limit = limit;
}

/* Ensure that spf is at least valid up to 'needed' (inclusive). */
static void ensure_spf(size_t needed)
{
    if (needed <= spf_limit) return;
    recompute_spf(needed);
}

/* Test whether n has exactly 4 positive divisors.
 * For n = p^3 or n = p*q with distinct primes p,q. */
static bool has4div(uint64_t n)
{
    if (n < 2) return false;
    uint64_t temp = n;
    uint32_t p = spf[n];
    int exp = 0;
    while (temp % p == 0) {
        temp /= p;
        exp++;
    }
    if (temp == 1) {               /* n = p^exp */
        return exp == 3;
    } else {                       /* n = p^1 * temp, temp must be prime, distinct from p */
        return exp == 1 && spf[temp] == temp && temp != p;
    }
}

/* ---------- Sequence storage ---------- */

static seq_t *terms = NULL;     /* dynamic array of found terms */
static size_t terms_cap = 0;    /* allocated capacity */
static size_t terms_len = 0;    /* number of terms stored */
static uint64_t max_checked = 0;/* last integer examined */

/* Grow the terms array when needed. */
static void ensure_terms_cap(void)
{
    size_t new_cap = terms_cap ? terms_cap * 2 : 64;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    terms = tmp;
    terms_cap = new_cap;
}

/* ---------- Main sequence function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, harmless */

    /* Initialise sieve once to a reasonable bound.
       1 000 000 is large enough to accommodate the first ~30 000 terms,
       which covers n up to at least 10 000 comfortably.     */
    if (spf_limit == 0) {
        ensure_spf(1000000);
    }

    /* Generate terms until we have at least n of them. */
    while (terms_len < (size_t)n) {
        uint64_t k = max_checked + 1;          /* next candidate */

        /* Make sure the sieve is large enough for k and k+1. */
        if (k + 1 > spf_limit) ensure_spf(k + 1);

        if (has4div(k) && has4div(k + 1)) {
            if (terms_len == terms_cap) ensure_terms_cap();
            terms[terms_len++] = (seq_t)k;
        }
        max_checked = k;
    }

    return terms[n - 1];
}
