/*
 * A072099  First term = 1 and, for each n > 1, n*a(n) does not appear.
 * https://oeis.org/A072099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 240, u16 n <= 2325, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A072099
   a(1) = 1.
   For n > 1, a(n) is the smallest positive integer that is
   not already a term and is not equal to k*a(k) for any k < n.
   Consequently n*a(n) is not one of the earlier terms.
   The function builds the sequence lazily and memoises results. */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* External integer types supplied by the build system */
extern seq_t *alloc_seq_array(size_t); /* provided helper, not used here */

/* --- Dynamic arrays for memoisation ----------------------------------- */

static seq_t *terms   = NULL;  /* stored sequence values               */
static size_t terms_cap   = 0;
static idx_t built = 0;        /* number of terms already computed    */

static uint8_t *used_term = NULL;   /* bitmap: 0=unused, 1=used term      */
static size_t   used_cap   = 0;

static seq_t *products = NULL;     /* sorted list of previous k*a(k)      */
static size_t   prod_cap   = 0;
static size_t   prod_cnt   = 0;

/* Ensure that terms[] can store at least 'need' elements.           */
static void ensure_terms_cap(size_t need)
{
    if (terms_cap >= need) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 4;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms = tmp;
    terms_cap = new_cap;
}

/* Ensure that used_term[] can be indexed up to 'need' (1‑based).     */
static void ensure_used_cap(size_t need)
{
    if (used_cap >= need) return;
    size_t new_cap = used_cap ? used_cap * 2 : 1024;
    while (new_cap < need) new_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(used_term, new_cap * sizeof(uint8_t));
    memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(uint8_t));
    used_term = tmp;
    used_cap   = new_cap;
}

/* Ensure that products[] can store at least 'need' elements.          */
static void ensure_prod_cap(size_t need)
{
    if (prod_cap >= need) return;
    size_t new_cap = prod_cap ? prod_cap * 2 : 4;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(products, new_cap * sizeof(seq_t));
    products = tmp;
    prod_cap = new_cap;
}

/* Binary search to test whether 'val' is present in products[].        */
static bool in_products(seq_t val)
{
    size_t lo = 0, hi = prod_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        seq_t midv = products[mid];
        if (midv == val) return true;
        if (midv < val) lo = mid + 1;
        else            hi = mid;
    }
    return false;
}

/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, return 0 */

    /* Return cached value if already known. */
    if (n <= built) return terms[n - 1];

    /* Grow storage for new terms. */
    ensure_terms_cap(n);

    /* Compute terms up to index n. */
    for (idx_t cur = built + 1; cur <= n; ++cur) {
        /* Candidate starts after the last term. */
        seq_t candidate = (built > 0) ? (terms[built - 1] + (seq_t)1) : (seq_t)1;

        /* Find the first acceptable candidate. */
        for (;;) {
            size_t candIdx = (size_t)candidate;
            ensure_used_cap(candIdx + 1);   /* assure we can index it */

            if (used_term[candIdx]) {      /* already used as a term */
                ++candidate;
                continue;
            }
            if (in_products(candidate)) {   /* equal to some previous k*a(k) */
                ++candidate;
                continue;
            }
            break;                         /* found a valid candidate */
        }

        /* Store the new term. */
        terms[built] = candidate;
        size_t candIdx = (size_t)candidate;
        ensure_used_cap(candIdx + 1);
        used_term[candIdx] = 1;

        /* Record its product for future exclusion. */
        seq_t prod = ((seq_t)cur) * candidate;
        ensure_prod_cap(prod_cnt + 1);
        products[prod_cnt++] = prod;

        ++built;
    }

    return terms[n - 1];
}
