/*
 * A062770  n/[largest power of squarefree kernel] equals 1; perfect powers of
 *          sqf-kernels (or sqf-numbers).
 * https://oeis.org/A062770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 176, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the n‑th smallest positive integer m whose prime factor
   exponents are all equal (i.e., m = K^e for some square‑free K).  The
   implementation performs a linear search, generating valid terms one by one
   and caching them so that later calls are O(1). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static bool   initialized   = false;
static seq_t *terms         = NULL;
static size_t term_len      = 0;
static size_t term_cap      = 0;
static idx_t  last_checked  = 1;               /* last integer examined */

static uint32_t *spf          = NULL;         /* smallest prime factor array */
static idx_t    spf_limit     = 0;             /* current max index for spf */

/* add a new term to the cache */
static void add_term(seq_t val)
{
    if (term_len == term_cap) {
        term_cap = term_cap ? term_cap * 2 : 8;
        terms = (seq_t *)realloc(terms, term_cap * sizeof(seq_t));
    }
    terms[term_len++] = val;
}

/* Allocate or extend the SPF array up to <limit> and fill missing entries. */
static void build_spf(idx_t limit)
{
    if (limit <= spf_limit) return;

    spf = (uint32_t *)realloc(spf, (limit + 1) * sizeof(uint32_t));
    for (idx_t i = spf_limit + 1; i <= limit; ++i) spf[i] = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                    /* i is prime */
            spf[i] = (uint32_t)i;
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
    spf_limit = limit;
}

/* Return true if m has equal exponents in its prime factorization. */
static bool good_candidate(uint64_t m)
{
    uint64_t x      = m;
    int     first_e = -1;
    while (x > 1) {
        uint32_t p = spf[x];
        int      e = 0;
        while (x % p == 0) { x /= p; ++e; }
        if (first_e == -1) first_e = e;
        else if (e != first_e) return false;
    }
    return true;
}

/* One‑time setup: build a modest SPF table. */
static void init(void)
{
    build_spf(2000000);
    initialized = true;
}

/* Return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined, safety fallback */
    if (!initialized) init();

    /* Generate terms until we have at least n of them. */
    while (term_len < n) {
        ++last_checked;
        if (last_checked > spf_limit) {
            /* enlarge the SPF table a bit; safe for n <= 10000 */
            build_spf(last_checked * 2);
        }
        if (good_candidate((uint64_t)last_checked))
            add_term((seq_t)last_checked);
    }
    return terms[n - 1];
}
