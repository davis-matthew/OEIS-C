/*
 * A062101  Squarefree numbers with every digit a squarefree number.
 * https://oeis.org/A062101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 4588, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   has_allowed_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:   Squarefree numbers with every digit a squarefree number.
   Computation: A sieve is built to determine squarefreeness quickly.
   For each candidate number we check that every decimal digit is in
   {1,2,3,5,6,7}.  The sequence is generated on demand and cached, so
   any order of calls to a(idx_t n) works.  All arithmetic is done in
   64‑bit or less to keep the result exact. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_allowed_digit.h"

/* ---------- static state ----------
 *  terms      : array of computed terms
 *  terms_len  : number of terms stored
 *  max_limit  : current upper bound of the sieve
 *  spf        : smallest prime factor table up to max_limit
 *  spf_limit  : current size of spf table
 */
static idx_t    terms_len = 0;
static seq_t   *terms    = NULL;    /* dynamic array of terms           */
static uint32_t max_limit = 1000000; /* initial sieve limit (1e6)        */
static uint32_t *spf      = NULL;    /* SPF table                        */
static uint32_t spf_limit = 0;       /* size of SPF table                */

static bool is_squarefree_uint32(uint32_t n)
{
    while (n > 1) {
        uint32_t p = spf[n];
        if (!p) return true; /* should not happen */
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (n % pp == 0)
            return false;               /* square factor found              */
        n /= p;
    }
    return true;
}

/* Rebuild the SPF table up to 'limit' (inclusive).  Simple quadratic sieve. */
static void build_spf(uint32_t limit)
{
    if (spf) free(spf);
    spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    for (uint32_t i = 0; i <= limit; ++i) spf[i] = 0;
    spf[0] = 0;
    spf[1] = 1;
    for (uint32_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if ((uint64_t)i * i <= limit) {
                for (uint32_t j = i * i; j <= limit; j += i) {
                    if (spf[j] == 0) spf[j] = i;
                }
            }
        }
    }
    spf_limit = limit;
}

/* Ensure that we have at least 'need' terms.   Generates terms as needed. */
static void ensure_terms(idx_t need)
{
    if (terms_len >= need) return;

    if (!spf) build_spf(max_limit);

    uint32_t cur = 1;
    if (terms_len > 0) cur = (uint32_t)(terms[terms_len - 1]) + 1;

    while (terms_len < need) {
        if (cur > max_limit) {                     /* enlarge sieve if required */
            uint32_t new_limit = max_limit * 2;
            if (new_limit < cur * 2) new_limit = cur * 2;
            build_spf(new_limit);
            max_limit = new_limit;
        }

        while (!has_allowed_digit(cur) || !is_squarefree_uint32(cur))
            ++cur;                                /* skip invalid candidates */

        /* store the valid term */
        if (!terms) {
            terms = (seq_t *)malloc(64 * sizeof(seq_t));
            terms_len = 0;
        } else if (terms_len % 64 == 0) {
            terms = (seq_t *)realloc(terms, (terms_len + 64) * sizeof(seq_t));
        }
        terms[terms_len++] = cur;
        ++cur;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; undefined for 0 */
    ensure_terms(n);
    return terms[n - 1];
}
