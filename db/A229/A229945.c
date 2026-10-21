/*
 * A229945  Triangle read by rows in which row n lists the union of the primes
 *          <= n and the divisors of n.
 * https://oeis.org/A229945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, less
 * Formula:   a(n) is the n‑th element of the concatenated rows of a triangle.
 *            Row k contains all divisors of k together with all primes ≤ k,
 *            sorted increasingly. The sequence starts with a(1)=1, a(2)=1,
 *            a(3)=2, …
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------- */
/* Integer square‑root (floor) for idx_t                                 */
/* --------------------------------------------------------------------- */
static idx_t isqrt(idx_t n)
{
    idx_t lo = 0, hi = n < (idx_t)1 << 32 ? n : (idx_t)1 << 32; /* safe upper bound */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n)  {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

/* --------------------------------------------------------------------- */
/* Global state for memoisation                                           */
/* --------------------------------------------------------------------- */
static seq_t *terms = NULL;      /* flattened sequence */
static size_t terms_size = 0;    /* number of terms stored */
static size_t terms_cap  = 0;    /* allocated capacity */

static bool *is_prime = NULL;    /* sieve array up to current limit */
static idx_t sieve_sz = 0;       /* current sieve limit */

static bool *seen = NULL;        /* temporary marker array for a row */
static size_t seen_cap = 0;      /* allocated capacity for seen */

static idx_t next_row = 1;       /* next row number to generate */

/* --------------------------------------------------------------------- */
/* Ensure the sieve array covers at least 'limit'                        */
/* --------------------------------------------------------------------- */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_sz) return;

    size_t new_cap = (size_t)(limit + 1);
    bool *new_sieve = (bool *)malloc(new_cap * sizeof(bool));
    if (!new_sieve) exit(1);

    for (size_t i = 0; i < new_cap; ++i)
        new_sieve[i] = true;
    if (new_cap > 0) new_sieve[0] = false;
    if (new_cap > 1) new_sieve[1] = false;

    idx_t sq = isqrt(limit);
    for (idx_t p = 2; p <= sq; ++p) {
        if (new_sieve[(size_t)p]) {
            for (idx_t mult = p * p; mult <= limit; mult += p)
                new_sieve[(size_t)mult] = false;
        }
    }

    free(is_prime);
    is_prime = new_sieve;
    sieve_sz = limit;
}

/* --------------------------------------------------------------------- */
/* Ensure the temporary 'seen' array can hold indices up to 'size_needed'*/
/* --------------------------------------------------------------------- */
static void ensure_seen(idx_t size_needed)
{
    if ((size_t)size_needed <= seen_cap) return;
    size_t new_cap = (size_t)size_needed;
    bool *new_seen = (bool *)realloc(seen, new_cap * sizeof(bool));
    if (!new_seen) exit(1);
    seen = new_seen;
    seen_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Append a value to the flattened sequence                              */
/* --------------------------------------------------------------------- */
static void append_term(seq_t val)
{
    if (terms_size + 1 > terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 8;
        while (new_cap <= terms_size + 1) new_cap *= 2;
        seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!new_terms) exit(1);
        terms = new_terms;
        terms_cap = new_cap;
    }
    terms[terms_size++] = val;
}

/* --------------------------------------------------------------------- */
/* Generate rows until at least 'need' terms have been produced           */
/* --------------------------------------------------------------------- */
static void grow_sequence(size_t need)
{
    while (terms_size < need) {
        idx_t k = next_row;          /* current row number */

        /* Ensure temporary buffer */
        ensure_seen(k + 1);
        memset(seen, 0, (size_t)(k + 1) * sizeof(bool));

        /* Mark all divisors of k */
        for (idx_t d = 1; d <= k / d; ++d) {
            if (k % d == 0) {
                seen[(size_t)d] = true;
                idx_t other = k / d;
                if (other != d)
                    seen[(size_t)other] = true;
            }
        }

        /* Ensure we know primes up to k */
        ensure_sieve(k);

        /* Mark all primes ≤ k */
        for (idx_t p = 2; p <= k; ++p) {
            if (is_prime[(size_t)p])
                seen[(size_t)p] = true;
        }

        /* Append all marked numbers in increasing order */
        for (idx_t x = 1; x <= k; ++x) {
            if (seen[(size_t)x])
                append_term((seq_t)x);
        }

        ++next_row;
    }
}

/* --------------------------------------------------------------------- */
/* Public sequence function                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;  /* undefined; safe fallback */

    grow_sequence((size_t)n);
    return terms[(size_t)(n - 1)];
}
