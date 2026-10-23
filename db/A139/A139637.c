/*
 * A139637  If n = the k-th prime, then a(n) = the (k-1)th prime. If n = the
 *          k-th composite, then a(n) = the (k-1)th composite. a(2) = 1. a(4)
 *          = 0.
 * https://oeis.org/A139637
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   If n = the k‑th prime, then a(n) = the (k-1)th prime. If n = the
 *            k‑th composite, then a(n) = the (k-1)th composite. a(2) = 1,
 *            a(4) = 0. Index offset is 2
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* ----------  Static state  ---------- */
static bool        _initialized = false;
static size_t      _limit = 0;            /* current sieve limit (inclusive)   */
static bool        *_is_prime = NULL;     /* sieve flags, size _limit+1        */
static idx_t       *_prime_counts = NULL; /* prefix counts of primes          */
static idx_t       *_comp_counts = NULL;  /* prefix counts of composites      */
static seq_t       *_prime_vals = NULL;   /* list of primes, 0‑based           */
static seq_t       *_comp_vals  = NULL;   /* list of composites, 0‑based      */
static size_t      _prime_total = 0;
static size_t      _comp_total  = 0;

/* ----------  Helper: grow sieve to at least new_limit (inclusive).  ---------- */
static bool _sieve_to_limit(size_t new_limit)
{
    if (new_limit <= _limit) return true;            /* already enough */

    /* Allocate new arrays. */
    bool   *new_is_prime    = (bool*)malloc((new_limit + 1) * sizeof(bool));
    idx_t  *new_prime_counts= (idx_t*)malloc((new_limit + 1) * sizeof(idx_t));
    idx_t  *new_comp_counts = (idx_t*)malloc((new_limit + 1) * sizeof(idx_t));
    if (!new_is_prime || !new_prime_counts || !new_comp_counts)
        return false;                                /* allocation failed */

    /* Sieve of Eratosthenes. */
    for (size_t i = 0; i <= new_limit; ++i) new_is_prime[i] = true;
    if (new_limit >= 0) new_is_prime[0] = false;
    if (new_limit >= 1) new_is_prime[1] = false;
    for (size_t p = 2; p * p <= new_limit; ++p)
        if (new_is_prime[p])
            for (size_t q = p * p; q <= new_limit; q += p)
                new_is_prime[q] = false;

    /* Compute prefix counts and build value lists. */
    size_t prime_cap = new_limit / 10 + 10;          /* rough initial capacity */
    size_t comp_cap  = new_limit / 2  + 10;          /* rough initial capacity */
    seq_t *new_prime_vals = (seq_t*)malloc(prime_cap * sizeof(seq_t));
    seq_t *new_comp_vals  = (seq_t*)malloc(comp_cap  * sizeof(seq_t));
    if (!new_prime_vals || !new_comp_vals) {
        free(new_is_prime); free(new_prime_counts); free(new_comp_counts);
        return false;
    }

    size_t prime_cnt = 0;
    size_t comp_cnt  = 0;

    /* Prefix counts: primes. */
    new_prime_counts[0] = 0;
    if (new_limit >= 1) new_prime_counts[1] = 0;

    /* Prefix counts: composites. */
    new_comp_counts[0] = 0;
    if (new_limit >= 1) new_comp_counts[1] = 0;
    if (new_limit >= 2) new_comp_counts[2] = 0;
    if (new_limit >= 3) new_comp_counts[3] = 0;

    for (size_t x = 2; x <= new_limit; ++x) {
        new_prime_counts[x] = new_prime_counts[x-1] + (new_is_prime[x] ? 1 : 0);
        new_comp_counts[x]  = new_comp_counts[x-1]  + ((x >= 4 && !new_is_prime[x]) ? 1 : 0);
    }

    /* Build lists of prime and composite values. */
    for (size_t x = 2; x <= new_limit; ++x) {
        if (new_is_prime[x]) {
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                seq_t *tmp = (seq_t*)realloc(new_prime_vals, prime_cap * sizeof(seq_t));
                if (!tmp) return false;
                new_prime_vals = tmp;
            }
            new_prime_vals[prime_cnt++] = (seq_t)x;
        } else {
            if (x >= 4) {
                if (comp_cnt == comp_cap) {
                    comp_cap *= 2;
                    seq_t *tmp = (seq_t*)realloc(new_comp_vals, comp_cap * sizeof(seq_t));
                    if (!tmp) return false;
                    new_comp_vals = tmp;
                }
                new_comp_vals[comp_cnt++] = (seq_t)x;
            }
        }
    }

    /* Free old data and commit new data. */
    free(_is_prime);
    free(_prime_counts);
    free(_comp_counts);
    free(_prime_vals);
    free(_comp_vals);

    _is_prime      = new_is_prime;
    _prime_counts  = new_prime_counts;
    _comp_counts   = new_comp_counts;
    _prime_vals    = new_prime_vals;
    _comp_vals     = new_comp_vals;
    _prime_total   = prime_cnt;
    _comp_total    = comp_cnt;
    _limit         = new_limit;

    return true;
}

/* ----------  Main sequence function  ---------- */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                /* undefined below offset, safe fallback */

    /* Ensure data up to n is available. */
    size_t required = (size_t)n;
    if (!_initialized || required > _limit) {
        size_t new_limit = required * 2 + 1000;   /* generous cushion */
        if (!_sieve_to_limit(new_limit))
            return 0;                       /* allocation failure fallback */
        _initialized = true;
    }

    if (_is_prime[n]) {                  /* n is prime */
        idx_t k = _prime_counts[n];      /* 1‑based index of the prime */
        if (k <= 1) return (seq_t)1;     /* 0th prime is defined as 1 */
        /* k >= 2: previous prime is at index k-2 in prime_vals. */
        return _prime_vals[k - 2];
    } else {                              /* n is composite (>=4) */
        if (n < 4) return 0;             /* safety for 2,3, though not composite */

        /* _comp_counts[n] gives the count of composites <= n.
           The first composite (4) gets count 1, so subtract 1 to get 0‑based index. */
        idx_t comp_pos = _comp_counts[n] - 1;  /* 0‑based composite index */
        if (comp_pos == 0) return (seq_t)0;    /* 0th composite maps to 0 */
        /* Previous composite is at index comp_pos-1. */
        return _comp_vals[comp_pos - 1];
    }
}
