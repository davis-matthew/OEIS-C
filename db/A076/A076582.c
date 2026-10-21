/*
 * A076582  Triangular numbers which are 8-almost primes.
 * https://oeis.org/A076582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the n-th triangular number T_m = m*(m+1)/2 that has
 *            exactly 8 prime factors (with multiplicity)
 * Range:     u8 none, u16 n <= 9, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* seq_t and idx_t are defined by the build; they are unsigned integer types. */

/* Upper bound large enough to generate the first ~10 000 terms of the sequence
   (m will stay below 500 000).  The array size is 2 000 000 to be safe. */
#define SPF_LIMIT 2000000

static uint32_t *spf = NULL;          /* smallest prime factor */
static size_t spf_size = 0;

/* Build smallest prime factor array up to 'limit' (inclusive).  This function
   may be called only once because we choose a generous static upper bound. */
static void build_spf(void)
{
    spf = (uint32_t *)malloc((SPF_LIMIT + 1) * sizeof(uint32_t));
    if (!spf) abort();
    for (size_t i = 0; i <= SPF_LIMIT; ++i) spf[i] = 0;
    spf[0] = 0;
    spf[1] = 1;
    for (size_t i = 2; i <= SPF_LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * i <= SPF_LIMIT) {
                for (size_t j = i * i; j <= SPF_LIMIT; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
    spf_size = SPF_LIMIT + 1;
}

/* Count the number of prime factors (with multiplicity) of x, using the spf
   table.  The table must cover all numbers encountered. */
static unsigned count_omega_uint64(uint64_t x)
{
    unsigned cnt = 0;
    while (x > 1) {
        uint32_t p;
        if (x <= SPF_LIMIT) {
            p = spf[(size_t)x];
        } else {
            /* Should not happen with the chosen limit. */
            p = (uint32_t)x;
        }
        do {
            x /= p;
            ++cnt;
        } while (x % p == 0);
    }
    return cnt;
}

/* Cache of found terms. */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;

/* Current m to test next. */
static uint64_t next_m = 1;

/* Ensure the cache has capacity at least 'need'. */
static void ensure_terms_capacity(size_t need)
{
    if (terms_cap >= need) return;
    size_t new_cap = terms_cap == 0 ? 16 : terms_cap << 1;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    terms = new_arr;
    terms_cap = new_cap;
}

/* Append a triangular number to the terms cache. */
static void add_term(seq_t t)
{
    ensure_terms_capacity(terms_len + 1);
    terms[terms_len++] = t;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Initialize spf only once. */
    if (!spf) build_spf();

    /* Generate terms until we have at least n of them. */
    while (terms_len < (size_t)n) {
        uint64_t m = next_m++;

        /* Compute Ω(m) + Ω(m+1) - 1 (the minus 1 accounts for division by 2). */
        unsigned omega =
            count_omega_uint64(m) + count_omega_uint64(m + 1) - 1;

        if (omega == 8) {
            seq_t tri = (seq_t)m * ((seq_t)m + 1) / 2;
            add_term(tri);
        }
    }

    return terms[n - 1];
}
